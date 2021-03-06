#pragma once

#include <cstddef>

#include "moci_core/moci_core.hpp"

namespace moci
{
class DatagramSocket
{
public:
    using Buffer = Vector<std::uint8_t>;

public:
    DatagramSocket();
    ~DatagramSocket();

    bool Bind(std::string ip, int port);

    bool Write(std::string const& host, int port, Span<std::uint8_t> buffer);
    bool Write(std::string const& host, int port, Buffer const& buffer);
    bool Write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes);

    void Listen();

    void Shutdown();

    void SetMessageCallback(std::function<void(Buffer const&, size_t)> callback);

private:
    class Pimpl;
    Scope<Pimpl> pimpl_;
};

}  // namespace moci
