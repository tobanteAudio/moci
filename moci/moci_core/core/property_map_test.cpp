/**
 * @file property_tree_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_core/core/property_map.hpp"

TEST_CASE("moci_core: PropertyMapKeyTypes", "[core]")
{
    {
        auto const tree = moci::PropertyMap<std::uint32_t>();
        REQUIRE(tree.GetName() == "");
    }
    {
        auto const tree = moci::PropertyMap<std::uint64_t>();
        REQUIRE(tree.GetName() == "");
    }
    {
        auto const tree = moci::PropertyMap<char>();
        REQUIRE(tree.GetName() == "");
    }
}

TEST_CASE("moci_core: PropertyMapDefaultConstruct", "[core]")
{
    auto const tree = moci::PropertyMap<std::uint32_t>();
    REQUIRE(tree.GetName() == "");
}

TEST_CASE("moci_core: PropertyMapNamedConstruct", "[core]")
{
    auto const tree = moci::PropertyMap<std::uint32_t>("test");
    REQUIRE(tree.GetName() == "test");
}

TEST_CASE("moci_core: PropertyMapSetName", "[core]")
{
    auto tree = moci::PropertyMap<std::uint32_t>();
    REQUIRE(tree.GetName() == "");
    tree.SetName("test");
    REQUIRE(tree.GetName() == "test");
}

TEST_CASE("moci_core: PropertyMapGetProperty", "[core]")
{
    auto tree = moci::PropertyMap<std::uint32_t>();
    tree.SetProperty(143, "test");
    auto const p = tree.GetProperty(143).value();
    REQUIRE(std::get<std::string>(p) == "test");
    auto const empty = tree.GetProperty(1);
    REQUIRE(empty.has_value() == false);
}

TEST_CASE("moci_core: PropertyMapSetProperty", "[core]")
{
    auto tree = moci::PropertyMap<std::uint32_t>();
    tree.SetProperty(143, "test");
    auto const p = tree.GetProperty(143).value();
    REQUIRE(std::get<std::string>(p) == "test");
}

TEST_CASE("moci_core: PropertyMapDeleteProperty", "[core]")
{
    auto tree = moci::PropertyMap<std::uint32_t>();

    tree.SetProperty(143, "test");
    REQUIRE(tree.HasKey(143) == true);

    auto const wasDeleted = tree.DeleteProperty(143);
    REQUIRE(wasDeleted == true);
    REQUIRE(tree.HasKey(143) == false);
}

TEST_CASE("moci_core: PropertyMapHasProperty", "[core]")
{
    auto tree = moci::PropertyMap<std::uint32_t>();
    REQUIRE(tree.HasKey(143) == false);
    tree.SetProperty(143, "test");
    REQUIRE(tree.HasKey(143) == true);
}
