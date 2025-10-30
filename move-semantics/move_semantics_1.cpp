#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("reference binding")
{
    std::string name = "jan"; // lvalue

    SECTION("C++98")
    {
        std::string& ref_name = name;
        const std::string& ref_full_name = full_name(name, "Kowalski");
        // ref_full_name[0] = 'J';
    }

    SECTION("C++11")
    {
        std::string&& rvalue_ref_full_name = full_name(name, "Kowalski");
        rvalue_ref_full_name[0] = 'J';
        std::cout << rvalue_ref_full_name << "\n";

        // std::string&& rvalue_ref_name = name;  // ERROR
    }
}

namespace StdExplain
{
    template <typename T>
    class Vector
    {
    public:
        void push_back(const T& val)
        {
            std::cout << "push_back makes copy " << val << " into vector\n";
        }

        void push_back(T&& val)
        {
            std::cout << "push_back moves " << val << " into vector\n";
        }
    };
} // namespace StdExplain

TEST_CASE("moving objects")
{
    StdExplain::Vector<std::string> vec;

    std::string name = "jan";

    vec.push_back(name);                        // lvalue - copy
    vec.push_back(full_name(name, "Kowalski")); // rvalue - move
    // vec.push_back(static_cast<std::string&&>(name));  // xvalue - move
    vec.push_back(std::move(name)); // xvalue - move

    name = std::string("adam"); // move
}
