/**
 * @file scope_guard_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/core/scope_guard.hpp"

TEST_CASE("moci/core: MakeScopeGuard", "[core]")
{
    auto i     = 0;
    auto guard = moci::MakeScopeGuard([&i]() { REQUIRE(i == 10); });
    i          = 10;
}

TEST_CASE("moci/core: MakeScopeGuardMultiple", "[core]")
{
    auto i  = 0;
    auto g1 = moci::MakeScopeGuard([&i]() { REQUIRE(i == 20); });
    auto g2 = moci::MakeScopeGuard([&i]() mutable {
        REQUIRE(i == 10);
        i = 20;
    });

    i = 10;
}

TEST_CASE("moci/core: MakeScopeGuardNoRolebackOnAquireThrowThrow", "[core]")
{
    std::vector<int> vec1;
    std::vector<int> vec2;
    try
    {
        auto doesNOTRollbackIfAdquireThrows = [&]() {
            vec1.push_back(42);
            auto a = moci::MakeScopeGuard([&]() {
                REQUIRE(vec1.size() > 0);
                vec1.pop_back();
            });

            auto b = moci::MakeScopeGuardThatDoesNOTRollbackIfAdquireThrows([&]() { vec2.push_back(42); },
                                                                            [&]() {
                                                                                REQUIRE(vec1.size() > 0);
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
        REQUIRE(vec1.size() == 0);
        REQUIRE(vec2.size() == 0);
    }
}

TEST_CASE("moci/core: MakeScopeGuardNoRolebackOnAquireThrowNoThrow", "[core]")
{
    std::vector<int> vec1;
    std::vector<int> vec2;
    auto doesNOTRollbackIfAdquireThrows = [&]() {
        vec1.push_back(42);
        auto a = moci::MakeScopeGuard([&]() {
            REQUIRE(vec1.size() > 0);
            vec1.pop_back();
        });

        auto b = moci::MakeScopeGuardThatDoesNOTRollbackIfAdquireThrows([&]() { vec2.push_back(42); },
                                                                        [&]() {
                                                                            REQUIRE(vec1.size() > 0);
                                                                            vec2.pop_back();
                                                                        });
        b.commit();
        a.commit();
    };

    doesNOTRollbackIfAdquireThrows();
    REQUIRE(vec1.size() == 1);
    REQUIRE(vec2.size() == 1);
}

TEST_CASE("moci/core: MakeScopeGuardRolebackOnAquireThrow", "[core]")
{
    std::vector<int> vec1;
    std::vector<int> vec2;
    try
    {
        auto doesRollbackIfAdquireThrows = [&]() {
            vec1.push_back(42);
            auto a = moci::MakeScopeGuard([&]() {
                REQUIRE(vec1.size() > 0);
                vec1.pop_back();
            });

            auto b = moci::MakeScopeGuardThatDoesRollbackIfAdquireThrows(
                [&]() {
                    vec2.push_back(42);
                    throw 1;
                },
                [&]() {
                    REQUIRE(vec1.size() > 0);
                    vec2.pop_back();
                });
        };

        doesRollbackIfAdquireThrows();
    }
    catch (...)
    {
        REQUIRE(vec1.size() == 0);
        REQUIRE(vec2.size() == 0);
    }
}