#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

void foo(int* ptr)
{
    if (ptr != nullptr)
    {
        std::cout << "foo(ptr is not null)\n";
    }
    else
    {
        std::cout << "foo(ptr is null)\n";
    }
}

void foo(int value)
{
    std::cout << "foo(int)\n";
}

void foo(nullptr_t)
{
    std::cout << "foo(nullptr_t)\n";
}

TEST_CASE("nullptr")
{
    foo(nullptr);
    
    int* ptr = nullptr;
    foo(ptr);

    int* ptr_other{}; // ptr_other = nullptr
    REQUIRE(ptr_other == nullptr);
}

void print(std::string_view text)
{
    std::cout << "Text: " << text << "\n";
}

TEST_CASE("string literals")
{
    using namespace std::literals;

    const char* ctxt = R"(abcdef)"
"ghik";
    std::string str_txt = "abcdef";

    print(ctxt);

    auto atxt_1 = "abcd\0ef";   // const char*
    auto atxt_2 = "abcdef"s;  // std::string
    auto atxt_3 = "abcdef"sv; // std::string_view

    print(atxt_1);
    print(atxt_2);
    print(atxt_3);

    const char* path = R"(c:\nasz katalog\todo)";
    print(path);

    const char* multi_line_text_1 = R"(line1
line2
line3)";

    auto multi_line_text_2 = R"raw(line1
line2
line3)raw"s;

    REQUIRE(multi_line_text_1 == multi_line_text_2);
}