#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <array>

using namespace std;

template <typename T>
using Dictionary = std::map<std::string, T>;

using DictionaryInt = Dictionary<int>; // replacement for typedef

template <size_t N>
using ByteBuffer = std::array<std::byte, N>;

TEST_CASE("template aliases")
{
    // std::map<std::string, int> dict = { {"one", 1}, {"two", 2} };
    Dictionary<int> dict = { {"one", 1}, {"two", 2} };

    ByteBuffer<1024> buffer{};
}

// template variable

template <typename T>
constexpr T pi_v = 3.141592653589793238462643;

TEST_CASE("template variable")
{
    auto result_1 = pi_v<double>;

    auto result_2 = pi_v<float>;

    static_assert(std::is_same<decltype(result_1), double>::value);
    static_assert(std::is_same_v<decltype(result_1), double>);
}
