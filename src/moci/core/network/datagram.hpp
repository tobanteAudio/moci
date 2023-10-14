#pragma once

#include <moci/core/logging.hpp>

#include <cstddef>
#include <span>
#include <vector>

namespace moci {
class DatagramSocket
{
public:
    using Buffer = std::vector<std::uint8_t>;

    DatagramSocket();
    ~DatagramSocket();

    auto bind(std::string const& ip, int port) -> bool;

    auto write(std::string const& host, int port, std::span<std::uint8_t> buffer) -> bool;
    auto write(std::string const& host, int port, Buffer const& buffer) -> bool;
    auto
    write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes)
        -> bool;

    void listen();

    void shutdown();

    void setMessageCallback(std::function<void(Buffer const&, size_t)> const& callback);

private:
    class Pimpl;
    std::unique_ptr<Pimpl> _pimpl;
};

}  // namespace moci
