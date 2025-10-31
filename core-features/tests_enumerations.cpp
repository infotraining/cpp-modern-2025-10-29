#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

enum OldStyleDaysOfWeek : uint8_t
{
    Mon = 1,
    Tue,
    Wed,
    Thd,
    Fri,
    Sat,
    Sun
};

static_assert(sizeof(OldStyleDaysOfWeek) == 1);

TEST_CASE("old style enums")
{
    OldStyleDaysOfWeek day_of_week = Mon;
    day_of_week = static_cast<OldStyleDaysOfWeek>(5);
    REQUIRE(day_of_week == Fri);

    std::underlying_type_t<OldStyleDaysOfWeek> value = day_of_week; // C++11

    //auto integer_value = std::to_underlying(day_of_week); // C++23
}

enum class Coffee : uint8_t
{
    espresso,
    cappuccino,
    flat_white,
    americano,
    v60,
    chemex
};

namespace Cpp23
{
    template <typename TEnum>
    constexpr auto to_underlying(TEnum e) noexcept
    {
        return static_cast<std::underlying_type_t<TEnum>>(e);
    }
}

TEST_CASE("enum classes")
{
    Coffee coffee = Coffee::v60;

    coffee = static_cast<Coffee>(4);
    REQUIRE(coffee == Coffee::v60);

    auto value = static_cast<std::underlying_type_t<Coffee>>(coffee);
    REQUIRE(static_cast<std::underlying_type_t<Coffee>>(coffee) == 4);
    REQUIRE(Cpp23::to_underlying(coffee) == 4);
}

namespace [[deprecated]] Legacy
{
    namespace OldStyleDaysOfWeekAlt
    {
        enum type : uint32_t
        {
            Mon = 1,
            Tue,
            Wed,
            Thd,
            Fri,
            Sat,
            Sun
        };
    }
}

namespace SinceCpp11
{
    enum class OldStyleDaysOfWeek : uint8_t
    {
        Mon = 1,
        Tue,
        Wed,
        Thd,
        Fri,
        Sat,
        Sun
    };
}

TEST_CASE("since C++17")
{
    Coffee coffee{4};
    REQUIRE(coffee == Coffee::v60);
}

namespace StdExplain
{
    enum byte : uint8_t
    {
    };

    byte operator&(byte lhs, byte rhs)
    {
        return static_cast<byte>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
    }
}

TEST_CASE("char as bad byte")
{
    char c1 = 65;
    char c2 = 3;

    std::cout << (c1 + c2) << "\n";
    char result = ~(c1 & c2);

    std::cout << "result: " << result << "\n";
}

TEST_CASE("std::byte")
{
    std::byte b1{0b0000'00100};
    std::byte b2{42};
    std::cout << "Result: " << std::to_integer<int>(~(b1 & b2)) << "\n";
}
