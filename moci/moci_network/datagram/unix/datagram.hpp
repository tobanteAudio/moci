#pragma once

#include "moci_core/moci_core.hpp"

#if defined(MOCI_LINUX) || defined(MOCI_MAC)

#include "moci_network/datagram/datagram.hpp"

#include <cstdint>

#include "moci_core/core/vector.hpp"
#include <atomic>
#include <functional>
#include <string>
#include <thread>

namespace moci
{
class DatagramSocket::Pimpl
{

public:
    Pimpl() = default;
    ~Pimpl();

    bool Bind(std::string ip, int port);

    static bool Write(std::string const& host, int port, Span<std::uint8_t> buffer);
    static bool Write(std::string const& host, int port, DatagramSocket::Buffer const& buffer);
    static bool Write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes);

    void Listen();

    void Shutdown();

    void SetMessageCallback(std::function<void(DatagramSocket::Buffer const&, size_t)> callback)
    {
        messageCallback_ = callback;
    }

private:
    std::atomic<bool> isRunning_ {false};
    int socketDescriptor_ {};
    DatagramSocket::Buffer buffer_ {};
    std::thread listenerThread_;
    std::function<void(DatagramSocket::Buffer, size_t)> messageCallback_ = nullptr;
};
}  // namespace moci

#endif