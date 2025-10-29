#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>

struct Aggregate
{
    int a;
    double b;
    std::string c;
};

class Class
{
public:
    int a;
    double b;
    std::string c;

    Class(int a, double b, std::string c) : a{a}, b{b}, c{c} {}
};

TEST_CASE("initialization syntax before C++11")
{
    SECTION("prmitive types")
    {
        int a1 = 42;
        int a2(42);
        int a3 = int(42);
        //int a4(); // most vexing parse

        REQUIRE(a1 == 42);
        REQUIRE(a2 == 42);
        REQUIRE(a3 == 42);
    }

    SECTION("aggregates")
    {
        Aggregate agg1{};
        Aggregate agg2{42, 3.14};
        Aggregate agg3{42, 3.14, "text"};
        Aggregate agg4{.a = 42, .c = "text"}; // since C++20
    }

    SECTION("classes")
    {
        Class c1(142, 3.14, "text");
    }
}

TEST_CASE("uniform init syntax")
{
    SECTION("primitive types")
    {
        int a1 = 42;
        int a2{42};
        int a3 = int{42};
        int a4{}; 

        //char c1{a1}; // narrowing conversion is not allowed

        int* ptr_1 = nullptr;
        int* ptr_2{};
    }

    SECTION("aggregates")
    {
        Aggregate agg1{};
        Aggregate agg2{42, 3.14};
        Aggregate agg3{42, 3.14, "text"};
        Aggregate agg4{.a = 42, .c = "text"}; // since C++20
    }

    SECTION("classes")
    {
        Class c1(142, 3.14, "text");
        Class c2{142, 3.14, "text"};
    }

    SECTION("arrays & containers")
    {
        int native_arr[] = {1, 2, 4, 5}; // intialization of aggragate
        std::vector<int> vec = {1, 2, 3, 4}; // initializer_list

        std::map<int, std::string> dict = { {1, "one"}, {2, "two"} };
    }
}

TEST_CASE("initializer_list")
{
    std::initializer_list<int> lst = {1, 2, 3, 4, 5};
    auto alst = {1, 2, 3, 4, 5};

    auto numbers = {1.0, 2.3, 3.44, static_cast<double>(9)};
}