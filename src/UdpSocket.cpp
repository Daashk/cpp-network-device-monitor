#include "UdpSocket.h"

#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <utility>

UdpSocket::UdpSocket()
{
    socketFd_ = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketFd_ < 0)
    {
        throw std::runtime_error(
            "Failed to create UDP socket"
        );
    }
}

UdpSocket::~UdpSocket()
{
    if (socketFd_ >= 0)
    {
        close(socketFd_);
    }
}

UdpSocket::UdpSocket(UdpSocket&& other) noexcept
    : socketFd_(other.socketFd_)
{
    other.socketFd_ = -1;
}

UdpSocket& UdpSocket::operator=(UdpSocket&& other) noexcept
{
    if (this != &other)
    {
        if (socketFd_ >= 0)
        {
            close(socketFd_);
        }

        socketFd_ = other.socketFd_;
        other.socketFd_ = -1;
    }

    return *this;
}

int UdpSocket::fd() const
{
    return socketFd_;
}