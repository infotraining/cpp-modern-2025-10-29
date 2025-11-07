#include <algorithm>
#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

template <typename Container>
void print_all(const Container& container, std::string_view prefix)
{
    cout << prefix << ": [ ";
    for (const auto& item : container)
        cout << item << " ";
    cout << "]\n";
}

void beware_of_null_termination(std::string_view file_name)
{
    FILE* f = fopen(file_name.data(), "w+"); // UB
    fclose(f);
    
    auto file_name_str = std::string(file_name);
    FILE* f2 = fopen(file_name_str.c_str(), "w+"); 
    fclose(f2);

    FILE* f3 = fopen(std::string(file_name).c_str(), "w+"); // UB
    fclose(f3);
}

TEST_CASE("string_view")
{
    const char* ctxt = "C-Text";

    std::string_view sv1 = ctxt;
    std::cout << "sv1: " << sv1 << "\n";

    std::string_view sv2(ctxt, 3);
    REQUIRE(sv2 == "C-T");

    std::string str = "std::string";
    std::string_view sv3 = str;
    REQUIRE(sv3 == "std::string");

    std::string_view sv4(str.data() + 2, 4);
    REQUIRE(sv4 == "d::s" );

    std::vector vec = {1, 2, 3};
    print_all(vec, "vec");

    std::string name = "vector of ints";
    print_all(vec, name);
}

std::string_view get_token(std::string_view text)
{
    auto pos = text.find_first_of(" ");
    if (pos != std::string_view::npos)
        return std::string_view{text.data(), pos};
    return text;
}

TEST_CASE("tokenizing")
{
    std::string text = "abc def";

    auto token = get_token(text);
    REQUIRE(token == "abc");

    // auto evil_token = get_token("abc defgdfgdfgdfhggggggggggg"s); // UB
    // REQUIRE(evil_token == "abc");
}