#include <iostream>
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    const int port = 5000;
    const std::string serverIp = "127.0.0.1";

    int socketFd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketFd < 0)
    {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(
            AF_INET,
            serverIp.c_str(),
            &serverAddress.sin_addr) <= 0)
    {
        std::cerr << "Invalid server address\n";
        close(socketFd);
        return 1;
    }

    std::string message = "DEVICE01|42.1|-63|1";

    ssize_t bytesSent = sendto(
        socketFd,
        message.c_str(),
        message.size(),
        0,
        reinterpret_cast<sockaddr*>(&serverAddress),
        sizeof(serverAddress)
    );

    if (bytesSent < 0)
    {
        std::cerr << "Failed to send data\n";
        close(socketFd);
        return 1;
    }

    std::cout << "Sent: " << message << '\n';

    close(socketFd);

    return 0;
}