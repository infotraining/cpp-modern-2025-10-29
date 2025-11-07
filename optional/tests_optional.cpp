#include <algorithm>
#include <array>
#include <atomic>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("optional")
{
    SECTION("null semantics for value types")
    {
        std::optional<int> opt_int;
        REQUIRE(opt_int.has_value() == false);

        opt_int = 42;
        REQUIRE(opt_int.has_value() == true);

        opt_int = std::nullopt;
        REQUIRE(opt_int.has_value() == false);
    }

    SECTION("safe access")
    {
        std::optional<int> opt_int = 42;
        REQUIRE(opt_int.value() == 42);

        opt_int.reset();
        REQUIRE_THROWS_AS(opt_int.value(), std::bad_optional_access);
    }

    SECTION("unsafe access *")
    {
        std::optional<int> opt_int = 42;
        REQUIRE(*opt_int == 42);

        opt_int = std::nullopt;

        if (opt_int.has_value())
            REQUIRE(*opt_int == 42);

        if (opt_int != std::nullopt)
            REQUIRE(*opt_int == 42);

        if (opt_int)
            REQUIRE(*opt_int == 42);

        REQUIRE(opt_int.value_or(-1) == -1);
    }
}


