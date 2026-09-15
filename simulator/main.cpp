#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <vector>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "UdpSocket.h"

int main()
{
    const int port = 5000;
    const std::string serverIp = "127.0.0.1";

    UdpSocket socket;

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(
            AF_INET,
            serverIp.c_str(),
            &serverAddress.sin_addr) <= 0)
        {
            std::cerr << "Invalid server address\n";
            return 1;
        }

    std::vector<std::string> devices =
    {
        "DEVICE01",
        "DEVICE02",
        "DEVICE03"
    };

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());

    std::uniform_real_distribution<double> temperatureDistribution(
        35.0, 50.0
    );

    std::uniform_int_distribution<int> signalDistribution(
        -80, -50
    );

    unsigned int sequenceNumber = 1;

    while (true)
    {
        for (const std::string& deviceId : devices)
        {
            if (deviceId == "DEVICE03" &&
                sequenceNumber >= 6 &&
                sequenceNumber <= 10)
                {
                    continue;
                }
            
            double temperature =
                temperatureDistribution(generator);

            int signalStrength =
                signalDistribution(generator);

            std::string message =
                deviceId + "|" +
                std::to_string(temperature) + "|" +
                std::to_string(signalStrength) + "|" +
                std::to_string(sequenceNumber);

            ssize_t bytesSent = sendto(
                socket.fd(),
                message.c_str(),
                message.size(),
                0,
                reinterpret_cast<sockaddr*>(&serverAddress),
                sizeof(serverAddress)
            );

            if (bytesSent < 0)
            {
                std::cerr << "Failed to send data\n";
                return 1;
            }

            std::cout << "Sent: "
                      << message
                      << '\n';
        }

        ++sequenceNumber;

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    return 0;
}