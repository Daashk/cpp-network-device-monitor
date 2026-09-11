#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    const int port = 5000;

    int socketFd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketFd < 0)
    {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(
            socketFd,
            reinterpret_cast<sockaddr*>(&serverAddress),
            sizeof(serverAddress)) < 0)
    {
        std::cerr << "Failed to bind socket\n";
        close(socketFd);
        return 1;
    }

    std::cout << "Device Monitor is listening on port "
              << port << "...\n";

    char buffer[1024]{};

    ssize_t bytesReceived = recvfrom(
        socketFd,
        buffer,
        sizeof(buffer) - 1,
        0,
        nullptr,
        nullptr
    );

    if (bytesReceived < 0)
    {
        std::cerr << "Failed to receive data\n";
        close(socketFd);
        return 1;
    }

    buffer[bytesReceived] = '\0';

    std::cout << "Received: " << buffer << '\n';

    close(socketFd);

    return 0;
}