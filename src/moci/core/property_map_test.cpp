/**
 * @file property_tree_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "moci/core/property_map.hpp"

TEST_CASE("core: PropertyMapKeyTypes", "[core]")
{
    {
        auto const tree = moci::PropertyMap<std::uint32_t>();
        REQUIRE(tree.GetName().empty());
    }
    {
        auto const tree = moci::PropertyMap<std::uint64_t>();
        REQUIRE(tree.GetName().empty());
    }
    {
        auto const tree = moci::PropertyMap<char>();
        REQUIRE(tree.GetName().empty());
    }
}

TEST_CASE("core: PropertyMapDefaultConstruct", "[core]")
{
    auto const tree = moci::PropertyMap<std::uint32_t>();
    REQUIRE(tree.GetName().empty());
}

TEST_CASE("core: PropertyMapNamedConstruct", "[core]")
{
    auto const tree = moci::PropertyMap<std::uint32_t>("test");
    REQUIRE(tree.GetName() == "test");
}

TEST_CASE("core: PropertyMapSetName", "[core]")
{
    auto tree = moci::PropertyMap<std::uint32_t>();
    REQUIRE(tree.GetName().empty());
    tree.SetName("test");
    REQUIRE(tree.GetName() == "test");
}

TEST_CASE("core: PropertyMapGetProperty", "[core]")
{
    auto tree = moci::PropertyMap<std::uint32_t>();
    tree.SetProperty(143, "test");
    auto const p = tree.GetProperty(143).value();
    REQUIRE(std::get<std::string>(p) == "test");
    auto const empty = tree.GetProperty(1);
    REQUIRE(empty.has_value() == false);
}

TEST_CASE("core: PropertyMapSetProperty", "[core]")
{
    auto tree = moci::PropertyMap<std::uint32_t>();
    tree.SetProperty(143, "test");
    auto const p = tree.GetProperty(143).value();
    REQUIRE(std::get<std::string>(p) == "test");
}

TEST_CASE("core: PropertyMapDeleteProperty", "[core]")
{
    auto tree = moci::PropertyMap<std::uint32_t>();

    tree.SetProperty(143, "test");
    REQUIRE(tree.HasKey(143) == true);

    auto const wasDeleted = tree.DeleteProperty(143);
    REQUIRE(wasDeleted == true);
    REQUIRE(tree.HasKey(143) == false);
}

TEST_CASE("core: PropertyMapHasProperty", "[core]")
{
    auto tree = moci::PropertyMap<std::uint32_t>();
    REQUIRE(tree.HasKey(143) == false);
    tree.SetProperty(143, "test");
    REQUIRE(tree.HasKey(143) == true);
}
