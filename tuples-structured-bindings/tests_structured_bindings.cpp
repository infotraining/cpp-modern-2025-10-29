#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

std::tuple<int, int, double> calc_stats(const std::vector<int>& data)
{
    std::vector<int>::const_iterator min_pos, max_pos;
    std::tie(min_pos, max_pos) = std::minmax_element(data.begin(), data.end());

    double avg = accumulate(data.begin(), data.end(), 0.0) / data.size();

    return std::make_tuple(*min_pos, *max_pos, avg);
}

namespace SinceCpp17
{
    template <typename TContainer>
    std::tuple<int, int, double> calc_stats(const TContainer& data)
    {
        auto [min_pos, max_pos] = std::minmax_element(std::begin(data), std::end(data));
        double avg = accumulate(std::begin(data), std::end(data), 0.0) / std::size(data);

        return {*min_pos, *max_pos, avg};
    }
} // namespace SinceCpp17

TEST_CASE("Before C++17")
{
    std::vector<int> data = {4, 42, 665, 1, 123, 13};

    // auto stats = calc_stats(data);
    // int min = std::get<0>(stats);
    // int max = std::get<1>(stats);
    // double avg = std::get<2>(stats);

    int min, max;
    double avg;
    std::tie(min, max, avg) = calc_stats(data);

    REQUIRE(min == 1);
    REQUIRE(max == Catch::Approx(665));
    REQUIRE(avg == Catch::Approx(141.333));
}

TEST_CASE("Since C++17")
{
    std::vector<int> data = {4, 42, 665, 1, 123, 13};

    const auto [min, max, avg] = calc_stats(data); // Structured Bindings

    REQUIRE(min == 1);
    REQUIRE(max == Catch::Approx(665));
    REQUIRE(avg == Catch::Approx(141.333));
}

TEST_CASE("iterating over maps")
{
    std::map<int, std::string> dict = { {1, "one"}, {2, "two"} };

    for(const auto& [key, value] : dict)
    {
        std::cout << key << " - " << value << "\n";
    }
}