#pragma once

#include "moci/core/logging.hpp"

#if defined(MOCI_LINUX) || defined(MOCI_MAC)

#include "moci/network/datagram/datagram.hpp"

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

    auto Bind(const std::string& ip, int port) -> bool;

    static auto Write(std::string const& host, int port, Span<std::uint8_t> buffer) -> bool;
    static auto Write(std::string const& host, int port, DatagramSocket::Buffer const& buffer) -> bool;
    static auto Write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes) -> bool;

    void Listen();

    void Shutdown();

    void SetMessageCallback(const std::function<void(DatagramSocket::Buffer const&, size_t)>& callback)
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