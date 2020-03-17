/**
 * @file info_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/system/info.hpp"

#include "moci/core/preprocessor.hpp"

#if defined(MOCI_LINUX)
TEST_CASE("moci/system: SystemInfoOSName", "[system]") { REQUIRE(moci::SystemInfo::GetOSName() == "Linux"); }
#endif

#if defined(MOCI_MAC)
TEST_CASE("moci/system: SystemInfoOSName", "[system]") { REQUIRE(moci::SystemInfo::GetOSName() == "Darwin"); }
#endif

#if defined(MOCI_WINDOWS)
TEST_CASE("moci/system: SystemInfoOSName", "[system]") { REQUIRE(moci::SystemInfo::GetOSName() == "Windows"); }
#endif

TEST_CASE("moci/system: SystemInfoDummyChecks", "[system]")
{
    // REQUIRE(moci::SystemInfo::GetVendor().empty() == false);
    // REQUIRE(moci::SystemInfo::GetCPUModel().empty() == false);
    REQUIRE(moci::SystemInfo::GetCPUCoreCount() != 0);
    REQUIRE(moci::SystemInfo::GetCPUThreadCount() != 0);
    // REQUIRE(moci::SystemInfo::GetCPUFeatures().empty() == false);
}
