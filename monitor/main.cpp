#include <iostream>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>

#include "Telemetry.h"
#include "DeviceState.h"
#include "ThreadSafeQueue.h"
#include "UdpSocket.h"

int main()
{
    const int port = 5000;

    UdpSocket socket;

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(
            socket.fd(),
            reinterpret_cast<sockaddr*>(&serverAddress),
            sizeof(serverAddress)) < 0)
    {
        std::cerr << "Failed to bind socket\n";
        return 1;
    }

    std::cout << "Device Monitor is listening on port "
              << port << "...\n";

    std::unordered_map<std::string, DeviceState> devices;

    ThreadSafeQueue<std::string> messageQueue;

    const auto offlineTimeout =
        std::chrono::seconds(3);

    std::thread receiverThread(
        [&]()
        {
            char buffer[1024]{};

            while (true)
            {
                ssize_t bytesReceived = recvfrom(
                    socket.fd(),
                    buffer,
                    sizeof(buffer) - 1,
                    0,
                    nullptr,
                    nullptr
                );

                if (bytesReceived < 0)
                {
                    continue;
                }

                buffer[bytesReceived] = '\0';

                messageQueue.push(
                    std::string(buffer, bytesReceived)
                );
            }
        }
    );

    while (true)
    {
        std::string message;

        bool received = messageQueue.waitPopFor(
            message,
            std::chrono::milliseconds(500)
        );

        auto now =
            std::chrono::steady_clock::now();

        if (received)
        {
            try
            {
                Telemetry telemetry =
                    parseTelemetry(message);

                auto deviceIt =
                    devices.find(telemetry.deviceId);

                if (deviceIt == devices.end())
                {
                    devices.emplace(
                        telemetry.deviceId,
                        DeviceState{
                            telemetry,
                            now,
                            true
                        }
                    );

                    std::cout
                        << telemetry.deviceId
                        << " is ONLINE\n";
                }
                else
                {
                    bool wasOffline =
                        !deviceIt->second.online;

                    deviceIt->second.telemetry =
                        telemetry;

                    deviceIt->second.lastSeen = now;
                    deviceIt->second.online = true;

                    if (wasOffline)
                    {
                        std::cout
                            << telemetry.deviceId
                            << " is ONLINE again\n";
                    }
                }
            }
            catch (const std::exception& error)
            {
                std::cerr
                    << "Invalid telemetry: "
                    << error.what()
                    << '\n';
            }
        }

        for (auto& [deviceId, state] : devices)
        {
            if (state.online &&
                isOffline(state, now, offlineTimeout))
            {
                state.online = false;

                std::cout
                    << "*** "
                    << deviceId
                    << " is OFFLINE ***\n";
            }
        }
    }

    return 0;
}