#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("class template argument deduction - tuple")
{
    std::tuple tpl{1, 3.14, "text"s};
}