#pragma once

#include <moci/core/logging.hpp>

#if defined(MOCI_WINDOWS)

    #include <moci/core/network/datagram.hpp>

    #include <cstdint>

    #include <atomic>
    #include <functional>
    #include <string>
    #include <thread>
    #include <vector>

namespace moci {
class DatagramSocket::Pimpl
{

public:
    Pimpl() = default;
    ~Pimpl();

    bool bind(std::string ip, int port);

    bool write(std::string const& host, int port, std::span<std::uint8_t> buffer);
    bool write(std::string const& host, int port, DatagramSocket::Buffer const& buffer);
    bool
    write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes);

    void listen();

    void shutdown();

    void
    setMessageCallback(std::function<void(DatagramSocket::Buffer const&, size_t)> callback)
    {
        messageCallback_ = callback;
    }

private:
    std::atomic<bool> isRunning_{false};
    unsigned long long socketDescriptor_{};
    DatagramSocket::Buffer buffer_{};
    std::thread listenerThread_;
    std::function<void(DatagramSocket::Buffer, size_t)> messageCallback_ = nullptr;
};
}  // namespace moci

#endif
