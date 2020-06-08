#include "moci/network/windows/datagram.hpp"

#if defined(MOCI_WINDOWS)

#include "moci_core/core/logging.hpp"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

namespace moci
{
DatagramSocket::Pimpl::~Pimpl() { }

bool DatagramSocket::Pimpl::Write(std::string const& host, int port, gsl::span<std::uint8_t> buffer)
{
    return Write(host, port, buffer.data(), buffer.size());
}

bool DatagramSocket::Pimpl::Write(std::string const& host, int port, DatagramSocket::Buffer const& buffer)
{
    return Write(host, port, buffer.data(), buffer.size());
}

bool DatagramSocket::Pimpl::Write(std::string const& host, int port, std::uint8_t const* const buffer, size_t numBytes)
{
    IgnoreUnused(host);
    IgnoreUnused(port);
    IgnoreUnused(buffer);
    IgnoreUnused(numBytes);
    return false;
}

bool DatagramSocket::Pimpl::Bind(std::string ip, int port)
{
    WSADATA wsa;
    auto rc = WSAStartup(MAKEWORD(2, 0), &wsa);
    if (rc != 0)
    {
        MOCI_CORE_ERROR("Could not start winsock: {}", rc);
        return false;
    }
    MOCI_CORE_INFO("Winsock start");

    socketDescriptor_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor_ == INVALID_SOCKET)
    {
        MOCI_CORE_ERROR("Could not create udp socket: {}", WSAGetLastError());
        return false;
    }

    MOCI_CORE_INFO("Created udp socket");

    SOCKADDR_IN addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(static_cast<unsigned short>(port));
    addr.sin_addr.s_addr = ADDR_ANY;

    rc = bind(socketDescriptor_, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
    if (rc == SOCKET_ERROR)
    {
        MOCI_CORE_ERROR("Could not bind to port: {}", WSAGetLastError());
        return false;
    }

    MOCI_CORE_INFO("Bound to port: {}", port);

    return true;
}

void DatagramSocket::Pimpl::Listen()
{
    listenerThread_ = std::thread([&]() {
        SOCKADDR_IN remoteAddr;
        int remoteAddrLen = sizeof(SOCKADDR_IN);
        std::memset(&remoteAddr, 0, sizeof(remoteAddr));

        auto const maxMsgSize = 1024;
        buffer_.resize(1024);

        MOCI_CORE_INFO("Start udp listen");
        isRunning_.store(true);
        while (isRunning_.load())
        {
            auto const rc = recvfrom(socketDescriptor_, reinterpret_cast<char*>(buffer_.data()),
                                     static_cast<int>(buffer_.size()), 0, (SOCKADDR*)&remoteAddr, &remoteAddrLen);
            if (rc == SOCKET_ERROR)
            {
                MOCI_CORE_ERROR("recvfrom, error code: {}", WSAGetLastError());
            }
            else
            {
                if (messageCallback_)
                {
                    messageCallback_(buffer_, buffer_.size());
                }
            }
        }
    });
}

void DatagramSocket::Pimpl::Shutdown()
{
    MOCI_CORE_INFO("Stop udp listen");
    isRunning_.store(false);
    if (listenerThread_.joinable())
    {
        listenerThread_.join();
    }
}

}  // namespace moci

#endif