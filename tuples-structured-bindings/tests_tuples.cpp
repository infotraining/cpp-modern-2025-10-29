#include <catch2/catch_test_macros.hpp>
#include <string>
#include <tuple>
#include <vector>

using namespace std::literals;

TEST_CASE("tuples")
{
    SECTION("Before C++17")
    {
        std::tuple<int, std::string, double> tpl = {42, "hello", 3.14};

        REQUIRE(std::get<0>(tpl) == 42);
        REQUIRE(std::get<1>(tpl) == "hello");
        REQUIRE(std::get<2>(tpl) == 3.14);
    }

    SECTION("Since C++17 - CTAD")
    {
        std::tuple tpl = {42, "hello"s, 3.14};

        REQUIRE(std::get<0>(tpl) == 42);
        REQUIRE(std::get<1>(tpl) == "hello");
        REQUIRE(std::get<2>(tpl) == 3.14);
    }
}

TEST_CASE("reference tuple")
{
    int x;
    double pi;
    std::string str;

    // std::tuple<int&, double&, std::string&> ref_tpl{x, pi, str};
    // ref_tpl = std::tuple{42, 3.14, "text"};

    std::tie(x, pi, str) = std::tuple{42, 3.14, "text"};

    REQUIRE(x == 42);
    REQUIRE(pi == 3.14);
    REQUIRE(str == "text");
}

struct Person
{
    std::string name;
    double height;
    int age;

    auto tied() const
    {
        return std::tie(name, height, age);
    }

    bool operator==(const Person& other) const
    {
        // return name == other.name && height == other.height && age == other.age;
        return tied() == other.tied();
    }

    bool operator!=(const Person& other) const
    {
        return !(*this == other);
    }

    bool operator<(const Person& other)
    {
        // if (name == other.name)
        // {
        //     if (height == other.height)
        //     {
        //         return age < other.age;
        //     }
        //     return height < other.height;
        // }
        // return name < other.name;

        return tied() < other.tied();
    }
};

TEST_CASE("Person - operators")
{
    Person p1{"Adam", 1.80, 33};
    Person p2{"Adam", 1.80, 33};
    Person p3{"Adam", 1.80, 34};

    REQUIRE(p1 == p2);
    REQUIRE(p1 < p3);
}
