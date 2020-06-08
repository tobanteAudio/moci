/**
 * @file datagram_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/network/datagram.hpp"
#include "moci_core/core/preprocessor.hpp"

#if defined(MOCI_LINUX) || defined(MOCI_MAC)

TEST_CASE("moci/network: UDPBindToPort", "[network]")
{
    moci::DatagramSocket udp {};
    REQUIRE(udp.Bind("", 0) == true);
}

#endif