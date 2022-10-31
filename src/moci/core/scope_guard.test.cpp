/**
 * @file scope_guard.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include <moci/core/scope_guard.hpp>
#include <moci/core/vector.hpp>

TEST_CASE("core: MakeScopeGuard", "[core]")
{
    auto i     = 0;
    auto guard = moci::makeScopeGuard([&i]() { REQUIRE(i == 10); });
    i          = 10;
}

TEST_CASE("core: MakeScopeGuardMultiple", "[core]")
{
    auto i  = 0;
    auto g1 = moci::makeScopeGuard([&i]() { REQUIRE(i == 20); });
    auto g2 = moci::makeScopeGuard(
        [&i]() mutable
        {
            REQUIRE(i == 10);
            i = 20;
        });

    i = 10;
}

TEST_CASE("core: MakeScopeGuardNoRolebackOnAquireThrowThrow", "[core]")
{
    moci::Vector<int> vec1;
    moci::Vector<int> vec2;
    try
    {
        auto doesNOTRollbackIfAdquireThrows = [&]()
        {
            vec1.push_back(42);
            auto a = moci::makeScopeGuard(
                [&]()
                {
                    REQUIRE(!vec1.empty());
                    vec1.pop_back();
                });

            auto b = moci::makeScopeGuardThatDoesNotRollbackIfAdquireThrows([&]() { vec2.push_back(42); },
                                                                            [&]()
                                                                            {
                                                                                REQUIRE(!vec1.empty());
                                                                                vec2.pop_back();
                                                                            });

            throw 1;
            // b.commit();
            // a.commit();
        };

        doesNOTRollbackIfAdquireThrows();
    }
    catch (...)
    {
        REQUIRE(vec1.empty());
        REQUIRE(vec2.empty());
    }
}

TEST_CASE("core: MakeScopeGuardNoRolebackOnAquireThrowNoThrow", "[core]")
{
    moci::Vector<int> vec1;
    moci::Vector<int> vec2;
    auto doesNOTRollbackIfAdquireThrows = [&]()
    {
        vec1.push_back(42);
        auto a = moci::makeScopeGuard(
            [&]()
            {
                REQUIRE(!vec1.empty());
                vec1.pop_back();
            });

        auto b = moci::makeScopeGuardThatDoesNotRollbackIfAdquireThrows([&]() { vec2.push_back(42); },
                                                                        [&]()
                                                                        {
                                                                            REQUIRE(!vec1.empty());
                                                                            vec2.pop_back();
                                                                        });
        b.commit();
        a.commit();
    };

    doesNOTRollbackIfAdquireThrows();
    REQUIRE(vec1.size() == 1);
    REQUIRE(vec2.size() == 1);
}

TEST_CASE("core: MakeScopeGuardRolebackOnAquireThrow", "[core]")
{
    moci::Vector<int> vec1;
    moci::Vector<int> vec2;
    try
    {
        auto doesRollbackIfAdquireThrows = [&]()
        {
            vec1.push_back(42);
            auto a = moci::makeScopeGuard(
                [&]()
                {
                    REQUIRE(!vec1.empty());
                    vec1.pop_back();
                });

            auto b = moci::makeScopeGuardThatDoesRollbackIfAdquireThrows(
                [&]()
                {
                    vec2.push_back(42);
                    throw 1;
                },
                [&]()
                {
                    REQUIRE(!vec1.empty());
                    vec2.pop_back();
                });
        };

        doesRollbackIfAdquireThrows();
    }
    catch (...)
    {
        REQUIRE(vec1.empty());
        REQUIRE(vec2.empty());
    }
}