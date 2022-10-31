/**
 * @file strings.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include <moci/core/strings.hpp>

TEST_CASE("core: StringsLeftTrim", "[core]")
{
    auto s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    moci::Strings::leftTrim(s);
    REQUIRE(s.size() == 6);
}

TEST_CASE("core: StringsLeftTrimCopy", "[core]")
{
    auto const s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    std::string newS(moci::Strings::leftTrimCopy(s));
    REQUIRE(newS.size() == 6);
}

TEST_CASE("core: StringsRightTrim", "[core]")
{
    auto s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    moci::Strings::rightTrim(s);
    REQUIRE(s.size() == 6);
}

TEST_CASE("core: StringsRightTrimCopy", "[core]")
{
    auto const s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    std::string newS(moci::Strings::rightTrimCopy(s));
    REQUIRE(newS.size() == 6);
}

TEST_CASE("core: StringsTrim", "[core]")
{
    auto s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    moci::Strings::trim(s);
    REQUIRE(s.size() == 5);
}

TEST_CASE("core: StringsTrimCopy", "[core]")
{
    auto const s = std::string(" HELLO ");
    REQUIRE(s.size() == 7);

    std::string newS(moci::Strings::trimCopy(s));
    REQUIRE(newS.size() == 5);
}

TEST_CASE("core: StringsSplitSemicolon", "[core]")
{
    auto const s      = std::string("1;2;3");
    auto const splits = moci::Strings::split(s, ';');
    REQUIRE(splits.size() == 3);
    REQUIRE(splits.at(0) == "1");
    REQUIRE(splits.at(1) == "2");
    REQUIRE(splits.at(2) == "3");
}

TEST_CASE("core: StringsSplitDash", "[core]")
{
    auto const s      = std::string("test1-test2-test3-test4");
    auto const splits = moci::Strings::split(s, '-');
    REQUIRE(splits.size() == 4);
    REQUIRE(splits.at(0) == "test1");
    REQUIRE(splits.at(1) == "test2");
    REQUIRE(splits.at(2) == "test3");
    REQUIRE(splits.at(3) == "test4");
}

TEST_CASE("core: StringsToInt", "[core]")
{
    REQUIRE(moci::Strings::toInt("1").value() == 1);
    REQUIRE(moci::Strings::toInt("-1").value() == -1);

    REQUIRE(moci::Strings::toInt("abc").has_value() == false);
    REQUIRE(moci::Strings::toInt("tyt-143").has_value() == false);
    REQUIRE(moci::Strings::toInt("-143tyt").has_value() == true);
    REQUIRE(moci::Strings::toInt("-143tyt").value() == -143);
}

TEST_CASE("core: StringsToIntegerTemplate", "[core]")
{
    REQUIRE(moci::Strings::toInteger<std::uint8_t>("1").value() == 1);
    REQUIRE(moci::Strings::toInteger<std::int8_t>("-1").value() == -1);

    REQUIRE(moci::Strings::toInteger<std::int64_t>("abc").has_value() == false);
    REQUIRE(moci::Strings::toInteger<std::int64_t>("tyt-143").has_value() == false);
    REQUIRE(moci::Strings::toInteger<std::int64_t>("-143tyt").has_value() == true);
    REQUIRE(moci::Strings::toInteger<std::int64_t>("-143tyt").value() == -143);
}