#pragma once

#include "moci/core/logging.hpp"

#if defined(MOCI_LINUX) || defined(MOCI_MAC)

#include "moci/core/network/datagram.hpp"

#include <cstdint>

#include "moci/core/vector.hpp"
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

    auto bind(const std::string& ip, int port) -> bool;

    static auto write(std::string const& host, int port, Span<std::uint8_t> buffer) -> bool;
    static auto write(std::string const& host, int port, DatagramSocket::Buffer const& buffer) -> bool;
    static auto write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes) -> bool;

    void listen();

    void shutdown();

    void setMessageCallback(const std::function<void(DatagramSocket::Buffer const&, size_t)>& callback)
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