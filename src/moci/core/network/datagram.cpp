#include "moci/core/network/datagram.hpp"

#include <utility>

#include "moci/core/logging.hpp"

#if defined(MOCI_MAC) || defined(MOCI_LINUX)
#include "moci/core/network/unix/datagram.hpp"
#elif defined(MOCI_WINDOWS)
#include "moci/core/network/windows/datagram.hpp"
#endif

namespace moci
{
DatagramSocket::DatagramSocket() { _pimpl = makeScope<Pimpl>(); }

DatagramSocket::~DatagramSocket() { _pimpl.reset(nullptr); }

auto DatagramSocket::bind(const std::string& ip, int port) -> bool { return _pimpl->bind(ip, port); }

auto DatagramSocket::write(std::string const& host, int port, Span<std::uint8_t> buffer) -> bool
{
    return _pimpl->write(host, port, buffer);
}

auto DatagramSocket::write(std::string const& host, int port, Buffer const& buffer) -> bool
{
    return _pimpl->write(host, port, buffer);
}

auto DatagramSocket::write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes) -> bool
{
    return _pimpl->write(host, port, buffer, numBytes);
}

void DatagramSocket::listen() { _pimpl->listen(); }

void DatagramSocket::shutdown() { _pimpl->shutdown(); }

void DatagramSocket::setMessageCallback(const std::function<void(Buffer const&, size_t)>& callback)
{
    _pimpl->setMessageCallback(callback);
}
}  // namespace moci