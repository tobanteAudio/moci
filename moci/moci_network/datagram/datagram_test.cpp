/**
 * @file datagram_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "moci/core/preprocessor.hpp"
#include "moci_network/moci_network.hpp"

#if defined(MOCI_LINUX) || defined(MOCI_MAC)

TEST_CASE("moci_network: UDPBindToPort", "[network]")
{
    moci::DatagramSocket udp {};
    REQUIRE(udp.Bind("", 0) == true);
}

#endif