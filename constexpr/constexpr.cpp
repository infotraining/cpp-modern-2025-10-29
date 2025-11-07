#include <array>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

constexpr int get_id()
{
    return 42;
}

constexpr uintmax_t factorial(uint64_t n)
{
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

template <size_t N, typename F>
constexpr auto create_lookup_table(F func)
{
    std::array<uintmax_t, N> result{};

    for (uintmax_t i = 0; i < N; ++i)
    {
        result[i] = func(i);
    }

    return result;
}

constexpr uintmax_t fibonacci(uintmax_t n)
{
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

TEST_CASE("constexpr")
{
    const int factor = 2;
    constexpr int i = factor * get_id();

    static_assert(i == 84);

    constexpr auto f_8 = factorial(8);

    int tab[factorial(4)] = {};

    constexpr auto factorial_lookup_table = create_lookup_table<20>(factorial);
    constexpr auto fibonacci_lookup_table = create_lookup_table<20>(fibonacci);
}

template <typename T>
T is_power_of_2(T value)
{
    if constexpr(std::is_integral_v<T>)
        return value > 0 && (value & (value - 1)) == 0;
    else
    {
        int exponent;
        const T mantissa = std::frexp(value, &exponent);
        return mantissa == static_cast<T>(0.5);
    }
}

template <size_t N>
auto create_buffer()
{
    if constexpr(N <= 1024)
    {
        return std::array<int, N>{};
    }
    else    
    {
        return std::vector<int>(N);
    }
}

// template <typename T>
double is_power_of_2(double value)
{
    int exponent;
    const double mantissa = std::frexp(value, &exponent);
    return mantissa == static_cast<double>(0.5);
}

TEST_CASE("constexpr if")
{
    REQUIRE(is_power_of_2(8));
    REQUIRE(is_power_of_2(64));
    REQUIRE(is_power_of_2(1024));
    REQUIRE(is_power_of_2(9) == false);

    REQUIRE(is_power_of_2(8.0));
    REQUIRE(is_power_of_2(64.0f));

    auto small_buffer = create_buffer<512>();
    auto large_buffer = create_buffer<2048>();
}