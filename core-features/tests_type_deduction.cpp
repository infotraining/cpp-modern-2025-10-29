#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std::literals;

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

TEST_CASE("auto type deduction")
{
    auto i = 42; // int

    static_assert(std::is_same_v<decltype(i), int>);
}

void foobar(int)
{
}

template <typename T>
void deduce_1(T arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template <typename T>
void deduce_2(T& arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

namespace Cpp20
{
    void deduce_1(auto arg)
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
    }
}

TEST_CASE("auto - type deduction - case 1")
{
    int x = 42;
    const int cx = 665;
    int &ref_x = x;
    const int &cref_x = x;
    int tab[10];

    auto ax1 = x; // int
    deduce_1(x);

    auto ax2 = cx;      // int - const is removed
    deduce_1(cx);

    auto ax3 = ref_x;   // int - ref is removed
    deduce_1(ref_x);

    auto ax4 = cref_x;  // int - ref to const is removed
    deduce_1(cref_x);

    auto ax5 = tab;     // int* - array decays to pointer
    deduce_1(tab);

    auto ax6 = foobar;  // void(*)(int) - function decays to pointer
    deduce_1(foobar);

    auto str = "text"s; // std::string
    deduce_1(str);
}

TEST_CASE("auto - type deduction - case 2")
{
    int x = 42;
    const int cx = 665;
    int &ref_x = x;
    const int &cref_x = x;
    int tab[10];

    auto& a1 = x;      // int&
    deduce_2(x);

    auto& a2 = cx;     // const int&
    deduce_2(cx);

    auto& a3 = ref_x;  // int&
    deduce_2(ref_x);

    auto& a4 = cref_x; // const int&
    deduce_2(cref_x);

    auto& a5 = tab;    // int(&)[10]
    deduce_2(tab);

    auto& a6 = foobar; // void(&)(int)
    deduce_2(foobar);
}

namespace StdExplain 
{
    template <typename T>
    constexpr size_t size(T& container)
    {
        return container.size();
    }

    template <typename T, size_t N>
    constexpr size_t size(T (&tab)[N])
    {
        return N;
    }
}

TEST_CASE("std::size")
{
    int tab1[1024] {};
    static_assert(StdExplain::size(tab1) == 1024);

    std::vector vec = {1, 2, 3};
    REQUIRE(StdExplain::size(vec) == 3);

    std::array arr = {1, 2, 3, 4, 5};
    static_assert(StdExplain::size(arr) == 5);
}