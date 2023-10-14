#pragma once

#include <moci/core/logging.hpp>

#if defined(MOCI_LINUX) || defined(MOCI_MAC)

#include <moci/core/network/datagram.hpp>

#include <cstdint>

#include <atomic>
#include <functional>
#include <vector>
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

    static auto write(std::string const& host, int port, std::span<std::uint8_t> buffer) -> bool;
    static auto write(std::string const& host, int port, DatagramSocket::Buffer const& buffer) -> bool;
    static auto write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes) -> bool;

    void listen();

    void shutdown();

    void setMessageCallback(const std::function<void(DatagramSocket::Buffer const&, size_t)>& callback)
    {
        _messageCallback = callback;
    }

private:
    std::atomic<bool> _isRunning {false};
    int _socketDescriptor {};
    DatagramSocket::Buffer _buffer {};
    std::thread _listenerThread;
    std::function<void(DatagramSocket::Buffer, size_t)> _messageCallback = nullptr;
};
}  // namespace moci

#endif
