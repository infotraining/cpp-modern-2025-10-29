#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace StdExplain
{
    template <typename... Ts>
    struct tuple
    {
    };

    template <typename T, typename... TArgs>
    std::unique_ptr<T> make_unique(TArgs&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<TArgs>(args)...));
    }
} // namespace StdExplain

template <typename... TTail>
void print_anything(const std::string& head, const TTail&... tail)
{

    std::cout << "'" << head << "' ";

    if constexpr (sizeof...(tail) > 0)
        print_anything(tail...);
    else
        std::cout << "\n";
}

template <typename THead, typename... TTail>
void print_anything(const THead& head, const TTail&... tail)
{

    std::cout << head << " ";

    if constexpr (sizeof...(tail) > 0)
        print_anything(tail...);
    else
        std::cout << "\n";
}

TEST_CASE("variadic templates")
{
    StdExplain::tuple<int, double, std::string> tpl_3;
    StdExplain::tuple<> tpl_0;

    print_anything(1, 3.14, "text"s);
    print_anything("text"s);

    auto ptr_general = std::make_unique<std::string>(4, '*');
    REQUIRE(*ptr_general == "****");
}

namespace FoldExpressions
{
    template <typename... TValues>
    auto sum(TValues... values)
    {
        return (... + values); // left-fold (((1 + 2) + 3) + 4)
    }

    template <typename... TArgs>
    void print_anything(const TArgs&... args)
    {
        (..., (std::cout << args << " ")) << "\n";
    }

    template<typename F, typename... TArgs>
    void call_for_each(F&& func, TArgs&&... args)
    {
        (..., func(std::forward<TArgs>(args)));
    }
} // namespace FoldExpressions 

TEST_CASE("fold expression")
{
    REQUIRE(FoldExpressions::sum(1, 2, 3, 4) == 10);
    REQUIRE(FoldExpressions::sum(1, 2, 3, 4, 5) == 15);

    FoldExpressions::print_anything(1, 3, 14, "text"s);
    auto printer = [](const auto& item) { std::cout << "item: " << item << "\n"; };

    FoldExpressions::call_for_each(printer, 1, 3.14, "text"s);
}