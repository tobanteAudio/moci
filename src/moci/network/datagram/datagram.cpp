#include "moci/network/datagram/datagram.hpp"

#include <utility>

#include "moci/core/logging.hpp"

#if defined(MOCI_MAC) || defined(MOCI_LINUX)
#include "moci/network/datagram/unix/datagram.hpp"
#elif defined(MOCI_WINDOWS)
#include "moci/network/datagram/windows/datagram.hpp"
#endif

namespace moci
{
DatagramSocket::DatagramSocket() { pimpl_ = makeScope<Pimpl>(); }

DatagramSocket::~DatagramSocket() { pimpl_.reset(nullptr); }

auto DatagramSocket::bind(const std::string& ip, int port) -> bool { return pimpl_->bind(ip, port); }

auto DatagramSocket::write(std::string const& host, int port, Span<std::uint8_t> buffer) -> bool
{
    return pimpl_->write(host, port, buffer);
}

auto DatagramSocket::write(std::string const& host, int port, Buffer const& buffer) -> bool
{
    return pimpl_->write(host, port, buffer);
}

auto DatagramSocket::write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes) -> bool
{
    return pimpl_->write(host, port, buffer, numBytes);
}

void DatagramSocket::listen() { pimpl_->listen(); }

void DatagramSocket::shutdown() { pimpl_->shutdown(); }

void DatagramSocket::setMessageCallback(const std::function<void(Buffer const&, size_t)>& callback)
{
    pimpl_->setMessageCallback(callback);
}
}  // namespace moci