/**
 * @file strings_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/core/strings.hpp"

TEST_CASE("moci/core: StringsLeftTrim", "[core]")
{
    auto s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    moci::Strings::LeftTrim(s);
    REQUIRE(s.size() == 6);
}

TEST_CASE("moci/core: StringsLeftTrimCopy", "[core]")
{
    auto const s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    std::string newS(moci::Strings::LeftTrimCopy(s));
    REQUIRE(newS.size() == 6);
}

TEST_CASE("moci/core: StringsRightTrim", "[core]")
{
    auto s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    moci::Strings::RightTrim(s);
    REQUIRE(s.size() == 6);
}

TEST_CASE("moci/core: StringsRightTrimCopy", "[core]")
{
    auto const s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    std::string newS(moci::Strings::RightTrimCopy(s));
    REQUIRE(newS.size() == 6);
}

TEST_CASE("moci/core: StringsTrim", "[core]")
{
    auto s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    moci::Strings::Trim(s);
    REQUIRE(s.size() == 5);
}

TEST_CASE("moci/core: StringsTrimCopy", "[core]")
{
    auto const s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    std::string newS(moci::Strings::TrimCopy(s));
    REQUIRE(newS.size() == 5);
}

TEST_CASE("moci/core: StringsSplitSemicolon", "[core]")
{
    auto const s      = std::string("1;2;3");
    auto const splits = moci::Strings::Split(s, ';');
    REQUIRE(splits.size() == 3);
    REQUIRE(splits.at(0) == "1");
    REQUIRE(splits.at(1) == "2");
    REQUIRE(splits.at(2) == "3");
}

TEST_CASE("moci/core: StringsSplitDash", "[core]")
{
    auto const s      = std::string("test1-test2-test3-test4");
    auto const splits = moci::Strings::Split(s, '-');
    REQUIRE(splits.size() == 4);
    REQUIRE(splits.at(0) == "test1");
    REQUIRE(splits.at(1) == "test2");
    REQUIRE(splits.at(2) == "test3");
    REQUIRE(splits.at(3) == "test4");
}

TEST_CASE("moci/core: StringsToInt", "[core]")
{
    REQUIRE(moci::Strings::ToInt("1").value() == 1);
    REQUIRE(moci::Strings::ToInt("-1").value() == -1);

    REQUIRE(moci::Strings::ToInt("abc").has_value() == false);
    REQUIRE(moci::Strings::ToInt("tyt-143").has_value() == false);
    REQUIRE(moci::Strings::ToInt("-143tyt").has_value() == true);
    REQUIRE(moci::Strings::ToInt("-143tyt").value() == -143);
}

TEST_CASE("moci/core: StringsToIntegerTemplate", "[core]")
{
    REQUIRE(moci::Strings::ToInteger<std::uint8_t>("1").value() == 1);
    REQUIRE(moci::Strings::ToInteger<std::int8_t>("-1").value() == -1);

    REQUIRE(moci::Strings::ToInteger<std::int64_t>("abc").has_value() == false);
    REQUIRE(moci::Strings::ToInteger<std::int64_t>("tyt-143").has_value() == false);
    REQUIRE(moci::Strings::ToInteger<std::int64_t>("-143tyt").has_value() == true);
    REQUIRE(moci::Strings::ToInteger<std::int64_t>("-143tyt").value() == -143);
}