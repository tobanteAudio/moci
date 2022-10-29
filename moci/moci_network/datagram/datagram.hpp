#pragma once

#include <cstddef>

#include "moci/core/logging.hpp"
#include "moci/core/span.hpp"
#include "moci/core/vector.hpp"

namespace moci
{
class DatagramSocket
{
public:
    using Buffer = Vector<std::uint8_t>;

    DatagramSocket();
    ~DatagramSocket();

    auto Bind(const std::string& ip, int port) -> bool;

    auto Write(std::string const& host, int port, Span<std::uint8_t> buffer) -> bool;
    auto Write(std::string const& host, int port, Buffer const& buffer) -> bool;
    auto Write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes) -> bool;

    void Listen();

    void Shutdown();

    void SetMessageCallback(const std::function<void(Buffer const&, size_t)>& callback);

private:
    class Pimpl;
    Scope<Pimpl> pimpl_;
};

}  // namespace moci
