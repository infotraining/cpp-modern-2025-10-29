#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "gadget.hpp"


std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("reference binding")
{
    std::string name = "jan";

    SECTION("C++98")
    {
        // TODO
    }

    SECTION("C++11")
    {
        // TODO
    }
}


TEST_CASE("moving objects")
{
    // TODO
}
