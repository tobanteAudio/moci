#include "datagram.hpp"

#if defined(MOCI_LINUX) || defined(MOCI_MAC)

    #include <moci/core/logging.hpp>

    #include <cassert>
    #include <cstdio>
    #include <cstdlib>
    #include <cstring>

    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>

namespace moci {
DatagramSocket::Pimpl::~Pimpl()
{
    shutdown();
    close(_socketDescriptor);
}

auto DatagramSocket::Pimpl::write(
    std::string const& host,
    int port,
    std::span<std::uint8_t> buffer
) -> bool
{
    return write(host, port, buffer.data(), buffer.size());
}

auto DatagramSocket::Pimpl::write(
    std::string const& host,
    int port,
    DatagramSocket::Buffer const& buffer
) -> bool
{
    return write(
        host,
        port,
        static_cast<std::uint8_t const*>(buffer.data()),
        buffer.size()
    );
}

auto DatagramSocket::Pimpl::write(
    std::string const& host,
    int port,
    std::uint8_t const* const buffer,
    size_t numBytes
) -> bool
{
    // Creating socket file descriptor
    int sockDescriptor = 0;
    if ((sockDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return false;
    }

    // Client
    sockaddr_in client{};
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port   = htons(static_cast<std::uint16_t>(port));
    if (inet_aton(host.c_str(), &client.sin_addr) == 0) {
        return false;
    }

    auto const bytesSend = sendto(
        sockDescriptor,
        buffer,
        numBytes,
        0,
        reinterpret_cast<sockaddr*>(&client),
        sizeof(client)
    );

    if (bytesSend == -1) {
        return false;
    }
    close(sockDescriptor);

    return true;
}

auto DatagramSocket::Pimpl::bind(std::string const& ip, int port) -> bool
{
    moci::ignoreUnused(ip);
    // Creating socket file descriptor
    if ((_socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return false;
    }

    sockaddr_in servaddr{};
    std::memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family      = AF_INET;  // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port        = htons(port);

    timeval tv{};
    tv.tv_sec  = 0;
    tv.tv_usec = 20'000;
    setsockopt(_socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, (char const*)&tv, sizeof tv);

    // Bind the socket with the server address
    return ::bind(
               _socketDescriptor,
               reinterpret_cast<sockaddr const*>(&servaddr),
               sizeof(servaddr)
           )
        >= 0;
}

void DatagramSocket::Pimpl::listen()
{
    _listenerThread = std::thread([&]() {
        sockaddr_in cliaddr{};
        std::memset(&cliaddr, 0, sizeof(cliaddr));

        auto const maxMsgSize = 1024;
        _buffer.resize(1024);

        MOCI_CORE_INFO("Start udp listen");
        _isRunning.store(true);
        while (_isRunning.load()) {
            unsigned int len        = 0;
            auto const numBytesRecv = recvfrom(
                _socketDescriptor,
                _buffer.data(),
                maxMsgSize,
                MSG_WAITALL,
                reinterpret_cast<sockaddr*>(&cliaddr),
                &len
            );

            if (numBytesRecv > 0) {
                if (_messageCallback) {
                    _messageCallback(_buffer, _buffer.size());
                }
            }
        }
    });
}

void DatagramSocket::Pimpl::shutdown()
{
    MOCI_CORE_INFO("Stop udp listen");
    _isRunning.store(false);
    if (_listenerThread.joinable()) {
        _listenerThread.join();
    }
}

}  // namespace moci

// /** \brief Wait for data to come in.
//  *
//  * This function waits for a given amount of time for data to come in. If
//  * no data comes in after max_wait_ms, the function returns with -1 and
//  * errno set to EAGAIN.
//  *
//  * The socket is expected to be a blocking socket (the default,) although
//  * it is possible to setup the socket as non-blocking if necessary for
//  * some other reason.
//  *
//  * This function blocks for a maximum amount of time as defined by
//  * max_wait_ms. It may return sooner with an error or a message.
//  *
//  * \param[in] msg  The buffer where the message will be saved.
//  * \param[in] max_size  The size of the \p msg buffer in bytes.
//  * \param[in] max_wait_ms  The maximum number of milliseconds to wait for a message.
//  *
//  * \return -1 if an error occurs or the function timed out, the number of bytes received
//  otherwise.
//  */
// int udp_server::timed_recv(char *msg, size_t max_size, int max_wait_ms)
// {
//     fd_set s;
//     FD_ZERO(&s);
//     FD_SET(f_socket, &s);
//     struct timeval timeout;
//     timeout.tv_sec = max_wait_ms / 1000;
//     timeout.tv_usec = (max_wait_ms % 1000) * 1000;
//     int retval = select(f_socket + 1, &s, &s, &s, &timeout);
//     if(retval == -1)
//     {
//         // select() set errno accordingly
//         return -1;
//     }
//     if(retval > 0)
//     {
//         // our socket has data
//         return ::recv(f_socket, msg, max_size, 0);
//     }

// // our socket has no data
// errno = EAGAIN;
// return -1;
// }

#endif
