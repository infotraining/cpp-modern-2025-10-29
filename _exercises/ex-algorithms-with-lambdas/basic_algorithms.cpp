#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

using namespace std;

TEST_CASE("lambda exercise")
{
    using namespace Catch::Matchers;

    vector<int> data = {1, 6, 3, 5, 8, 9, 13, 12, 10, 45};

    SECTION("count even numbers - std::count_if")
    {
        // auto evens_count = std::count_if(data.begin(), data.end(), [](int n) { return n % 2 == 0; });
        auto evens_count = std::ranges::count_if(data, [](int n) { return n % 2 == 0; });

        REQUIRE(evens_count == 4);
    }

    SECTION("copy evens to vector - std::copy_if")
    {
        vector<int> evens;

        // std::back_insert_iterator<vector<int>> pusher(evens);
        // *pusher = 10; // evens.push_back(10);
        // *pusher = 20; // evens.push_back(20);

        // std::copy_if(data.begin(), data.end(),
        //              std::back_inserter(evens),  [](int n) { return n % 2 == 0; });

        std::ranges::copy_if(data, std::back_inserter(evens), [](int n) { return n % 2 == 0; });

        REQUIRE_THAT(evens, Equals(vector<int>{6, 8, 12, 10}));
    }

    // SECTION("copy evens to vector - C++23")
    // {
    //     std::vector evens = data
    //         | std::views::filter([](int n) { return n % 2 == 0; })
    //         | std::ranges::to<vector>();
    // }

    SECTION("create container with squares - std::transform")
    {
        SECTION("ver_1")
        {
            vector<int> squares;

            std::transform(data.begin(), data.end(),
                std::back_inserter(squares), [](int value) { return value * value; });

            REQUIRE_THAT(squares, Equals(vector<int>{1, 36, 9, 25, 64, 81, 169, 144, 100, 2025}));
        }

        SECTION("ver_2")
        {
            vector<int> squares(data.size());

            std::transform(data.begin(), data.end(),
                squares.begin(), [](int value) { return value * value; });

            REQUIRE_THAT(squares, Equals(vector<int>{1, 36, 9, 25, 64, 81, 169, 144, 100, 2025}));
        }

        // SECTION("ver_3 - C++23")
        // {
        //     vector<int> squares = data
        //         | std::views::transform([](int value) { return value * value; })
        //         | std::ranges::to<vector>();

        //     REQUIRE_THAT(squares, Equals(vector<int>{1, 36, 9, 25, 64, 81, 169, 144, 100, 2025}));
        // }
    }

    SECTION("remove from container items divisible by any number from a given array - std::remove_if")
    {
        const array<int, 3> eliminators = {3, 5, 7};

        SECTION("ver_1")
        {
            auto garbage_start = std::remove_if(std::begin(data), std::end(data),
                [&eliminators](auto x) {
                    return std::any_of(std::begin(eliminators), std::end(eliminators),
                        [x](auto y) { return x % y == 0; });
                });

            data.erase(garbage_start, std::end(data));

            REQUIRE_THAT(data, Equals(vector<int>{1, 8, 13}));
        }

        SECTION("ver_2 - C++20")
        {
            std::erase_if(data,
                [&eliminators](auto x) {
                    return std::any_of(std::begin(eliminators), std::end(eliminators),
                        [x](auto y) { return x % y == 0; });
                });

            REQUIRE_THAT(data, Equals(vector<int>{1, 8, 13}));
        }
    }

    SECTION("calculate average - std::accumulate")
    {
        auto sum = 0.0;
        double avg = std::accumulate(data.begin(), data.end(), 0.0) / std::size(data);

        REQUIRE_THAT(avg, WithinAbs(11.2, 0.1));

        SECTION("create two containers - 1st with numbers less or equal to average & 2nd with numbers greater than average")
        {
            vector<int> less_equal_than_avg;
            vector<int> greater_than_avg;

            std::partition_copy(data.begin(), data.end(), 
                    std::back_inserter(less_equal_than_avg),
                    std::back_inserter(greater_than_avg),
                    [avg](int n) { return n <= avg;});

            REQUIRE_THAT(less_equal_than_avg, Equals(vector<int>{1, 6, 3, 5, 8, 9, 10}));
            REQUIRE_THAT(greater_than_avg, Equals(vector<int>{13, 12, 45}));
        }
    }
}