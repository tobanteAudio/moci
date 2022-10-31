/**
 * @file datagram.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "moci/core/network/datagram.hpp"
#include "moci/core/preprocessor.hpp"

#if defined(MOCI_LINUX) || defined(MOCI_MAC)

TEST_CASE("network: UDPBindToPort", "[network]")
{
    moci::DatagramSocket udp {};
    REQUIRE(udp.bind("", 0) == true);
}

#endif