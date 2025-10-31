#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

namespace Callables
{
    auto add(int a, int b) -> int
    {
        return a + b;
    }

    struct Add
    {
        int operator()(int a, int b) const
        {
            return a + b;
        }
    };

    auto lambda_add = [](int a, int b) {
        return a + b;
    };
}

TEST_CASE("callables")
{
    using namespace Callables;

    REQUIRE(add(1, 2) == 3);

    Add adder;
    REQUIRE(adder(1, 2) == 3);

    REQUIRE(lambda_add(1, 2) == 3);
}

struct Lambda_423423423234
{
    auto operator()(int a, int b) const 
    { 
        return a + b; 
    }; 
};

TEST_CASE("lambda expressions")
{
    auto add = [](int a, int b) { return a + b; };
    REQUIRE(add(1, 2) == 3);

    SECTION("is interpreted as")
    {
        auto add = Lambda_423423423234{};
        REQUIRE(add(1, 2) == 3);
    }

    SECTION("since C++20")
    {
        decltype(add) another_add;
        REQUIRE(another_add(1, 2) == 3);
    }
}

TEST_CASE("using decltype(lambda)")
{
    int x = 42;
    int y = 13;
    int z = 665;

    auto cmp_by_value = [](int* a, int* b) {
        return *a < *b;
    };

    std::set<int*, decltype(cmp_by_value)> mset({&x, &y, &z}, cmp_by_value);

    for(const int* ptr : mset)
    {
        std::cout << *ptr << " ";
    }
    std::cout << "\n";
}

TEST_CASE("explicit return type")
{
    auto describe = [](int n) -> std::string {
        if (n % 2 == 0)
            return "even";
        return std::string("odd");
    };

    REQUIRE(describe(2) == "even"s);
}

class Lambda_426575478568213
{
    int factor;

    auto operator()(int x) const { return x * factor; }
};

TEST_CASE("capturing local variables")
{
    int factor = 10;
    
    SECTION("by value")
    {
        auto multiply_by = [factor](int x) { 
            return x * factor;
        };
        
        factor = 100;
        REQUIRE(multiply_by(2) == 20);
    }
    
    SECTION("by ref")
    {
        int value{};

        auto increase_value = [&value]() { ++value; };
        increase_value();
        increase_value();

        REQUIRE(value == 2);
    }

    SECTION("multiple captures")
    {
        std::vector<int> vec = {1, 2, 3};

        auto pusher = [&vec, factor](int x) { 
            vec.push_back(x * factor);
        };

        pusher(10);
        pusher(11);
        pusher(12);

        REQUIRE(vec == std::vector{1, 2, 3, 100, 110, 120});
    }
}

class Printer
{
    std::string id;
public:
    explicit Printer(std::string id) : id(std::move(id))
    {}

    void info()
    {
        auto info_printer = [this]() { 
            std::cout << "Printer id: " << id << std::endl;
        };

        info_printer();
    }
};

TEST_CASE("capturing this")
{
    Printer prn_1("PRN#1");
    prn_1.info();
}

TEST_CASE("init captures")
{
    std::unique_ptr<int> ptr_factor = std::make_unique<int>(42);

    auto multiply_by = [ptr_factor = std::move(ptr_factor)](int x) { 
        return x * (*ptr_factor);
    };
}

auto create_generator(int seed)
{
    return [seed] mutable { return ++seed; };
}

TEST_CASE("mutable lambdas")
{
    int seed = 0;

    auto generate_value = [seed] mutable { return ++seed; };

    REQUIRE(generate_value() == 1);
    REQUIRE(generate_value() == 2);
    REQUIRE(generate_value() == 3);
    REQUIRE(generate_value() == 4);

    auto generate_large_values = create_generator(1'000'000);
    REQUIRE(generate_large_values() == 1'000'001);
    REQUIRE(generate_large_values() == 1'000'002);
    REQUIRE(generate_large_values() == 1'000'003);
}

TEST_CASE("immutable captured ref")
{
    int x = 10;

    auto safe_use = [&x = std::as_const(x)] { 
        std::cout << "x: " << x; };

    safe_use();
}

struct Lambda_369173465365
{
    template <typename T1, typename T2>
    auto operator()(T1 a, T2 b) const
    {
        return a + b;
    }
};

TEST_CASE("generic lambda")
{
    auto add = [](const auto& a, const auto& b) {
        return a + b;
    };

    REQUIRE(add(1, 2) == 3);
    REQUIRE(add("1"s, "2") == "12"s);
}