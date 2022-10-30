/**
 * @file info_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "moci/system/info.hpp"

#include "moci/core/preprocessor.hpp"

#if defined(MOCI_LINUX)
TEST_CASE("system: SystemInfoOSName", "[system]") { REQUIRE(moci::SystemInfo::getOSName() == "Linux"); }
#endif

#if defined(MOCI_MAC)
TEST_CASE("system: SystemInfoOSName", "[system]") { REQUIRE(moci::SystemInfo::getOSName() == "Darwin"); }
#endif

#if defined(MOCI_WINDOWS)
TEST_CASE("system: SystemInfoOSName", "[system]") { REQUIRE(moci::SystemInfo::getOSName() == "Windows"); }
#endif

TEST_CASE("system: SystemInfoDummyChecks", "[system]")
{
    // REQUIRE(moci::SystemInfo::GetVendor().empty() == false);
    // REQUIRE(moci::SystemInfo::GetCPUModel().empty() == false);
    REQUIRE(moci::SystemInfo::getCpuCoreCount() != 0);
    REQUIRE(moci::SystemInfo::getCpuThreadCount() != 0);
    // REQUIRE(moci::SystemInfo::GetCPUFeatures().empty() == false);
}
