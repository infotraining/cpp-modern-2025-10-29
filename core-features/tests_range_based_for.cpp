#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <map>
#include <set>

namespace StdExplain
{
    template <typename T>
    auto begin(T &container)
    {
        return container.begin();
    }

    template <typename T>
    auto end(T &container)
    {
        return container.end();
    }

    template <typename T, size_t N>
    auto begin(T (&tab)[N])
    {
        return tab;
    }

    template <typename T, size_t N>
    auto end(T (&tab)[N])
    {
        return tab + N;
    }
}

TEST_CASE("looping with auto")
{
    // std::vector<int> vec;
    // vec.push_back(1);
    // vec.push_back(2);
    // vec.push_back(3);
    int vec[10] = {1, 2, 3, 4, 5};

    for (auto it = std::begin(vec); it != std::end(vec); ++it)
    {
        *it *= 2;
    }

    for (auto it = std::cbegin(vec); it != std::cend(vec); ++it)
    {
        std::cout << *it << "\n";
    }
}

TEST_CASE("range-based for")
{
    std::vector<int> vec = {1, 2, 3, 4, 5};

    SECTION("for-each")
    {
        for (auto i : vec)
        {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }

    SECTION("is interpreted as")
    {
        auto &&container = vec;
        auto __begin = vec.begin(); // begin(vec) is also tried
        auto __end = vec.end();     // end(vec) is also tried
        for (auto it = __begin; it != __end; ++it)
        {
            int i = *it;
            std::cout << i << " ";
        }
        std::cout << "\n";
    }
}

TEST_CASE("range-based for works with")
{
    SECTION("std containers")
    {
        std::vector<int> vec = {1, 2, 3, 4, 5};

        for (auto i : vec)
        {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }

    SECTION("native arrays")
    {
        int native_tab[10] = {1, 2, 3, 4};

        for (auto &item : native_tab)
        {
            item *= 2;
        }

        std::cout << "\n";
    }

    SECTION("initializer lists")
    {
        auto lst = {1, 2, 3, 4, 5};
        for (const auto &item : lst)
        {
            std::cout << item << " ";
        }
        std::cout << "\n";

        for (const auto &item : {1, 2, 3})
            std::cout << item << " ";
        std::cout << "\n";
    }
}

TEST_CASE("beware of effeciency")
{
    std::vector<std::string> words = {"one", "two", "three"};

    for (auto &w : words)
    {
        w += "!!!";
    }

    for (const auto &w : words)
    {
        std::cout << w << " ";
    }
    std::cout << "\n";
}

TEST_CASE("iterating over map")
{
    std::map<int, std::string> dict = {{1, "one"}, {2, "two"}, {3, "three"}};

    SECTION("Std < C++17")
    {
        for (const auto& entry : dict)
        {
            std::cout << entry.first << " - " << entry.second << "\n";
        }
    }
    
    SECTION("Std >= C++17")
    {
        for(const auto& [key, value] : dict) // Structured Bindings
        {
            std::cout << key << " - " << value << "\n";
        }
    }
}

struct Point
{
    int x, y;
};

struct Triangle
{
    Point vertices[3];

    using iterator = Point*;
    using const_iterator = const Point*;

    iterator begin()
    {
        return vertices;
    }

    iterator end()
    {
        return vertices + 3;
    }

    const_iterator begin() const
    {
        return vertices;
    }

    const_iterator end() const
    {
        return vertices + 3;
    }
};

namespace AlternativeTake
{
    struct Triangle
    {
        Point vertices[3];
    };

    Point* begin(Triangle& t)
    {
        return t.vertices;
    }

    Point* end(Triangle& t)
    {
        return t.vertices + 3;
    }

    const Point* begin(const Triangle& t)
    {
        return t.vertices;
    }

    const Point* end(const Triangle& t)
    {
        return t.vertices + 3;
    }
}

TEST_CASE("iterating over Triangle")
{
    const Triangle triangle{Point{0, 0}, Point{0, 1}, Point{4, 4}};

    for(const auto& v : triangle)
    {
        std::cout << "(" << v.x << ", " << v.y << ")"; 
    }
}