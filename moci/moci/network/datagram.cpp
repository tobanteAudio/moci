#include "moci/network/datagram.hpp"

#include "moci/core/preprocessor.hpp"
#if defined(MOCI_MAC) || defined(MOCI_LINUX)
#include "moci/network/unix/datagram.hpp"
#elif defined(MOCI_WINDOWS)
#include "moci/network/windows/datagram.hpp"
#endif

namespace moci
{
DatagramSocket::DatagramSocket() { pimpl_ = MakeScope<Pimpl>(); }

DatagramSocket::~DatagramSocket() { pimpl_.reset(nullptr); }

bool DatagramSocket::Bind(std::string ip, int port) { return pimpl_->Bind(ip, port); }

bool DatagramSocket::Write(std::string const& host, int port, gsl::span<std::uint8_t> buffer)
{
    return pimpl_->Write(host, port, buffer);
}

bool DatagramSocket::Write(std::string const& host, int port, Buffer const& buffer)
{
    return pimpl_->Write(host, port, buffer);
}

bool DatagramSocket::Write(std::string const& host, int port, std::uint8_t const* buffer, size_t numBytes)
{
    return pimpl_->Write(host, port, buffer, numBytes);
}

void DatagramSocket::Listen() { pimpl_->Listen(); }

void DatagramSocket::Shutdown() { pimpl_->Shutdown(); }

void DatagramSocket::SetMessageCallback(std::function<void(Buffer const&, size_t)> callback)
{
    pimpl_->SetMessageCallback(callback);
}
}  // namespace moci