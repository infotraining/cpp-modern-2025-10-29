#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>

struct Aggregate
{
    int a;
    double b;
    std::string c;
};

class Class
{
public:
    int a;
    double b;
    std::string c;

    Class(int a, double b, std::string c) : a{a}, b{b}, c{c} {}
};

TEST_CASE("initialization syntax before C++11")
{
    SECTION("prmitive types")
    {
        int a1 = 42;
        int a2(42);
        int a3 = int(42);
        //int a4(); // most vexing parse

        REQUIRE(a1 == 42);
        REQUIRE(a2 == 42);
        REQUIRE(a3 == 42);
    }

    SECTION("aggregates")
    {
        Aggregate agg1{};
        Aggregate agg2{42, 3.14};
        Aggregate agg3{42, 3.14, "text"};
        Aggregate agg4{.a = 42, .c = "text"}; // since C++20
    }

    SECTION("classes")
    {
        Class c1(142, 3.14, "text");
    }
}

TEST_CASE("uniform init syntax")
{
    SECTION("primitive types")
    {
        int a1 = 42;
        int a2{42};
        int a3 = int{42};
        int a4{}; 

        //char c1{a1}; // narrowing conversion is not allowed

        int* ptr_1 = nullptr;
        int* ptr_2{};
    }

    SECTION("aggregates")
    {
        Aggregate agg1{};
        Aggregate agg2{42, 3.14};
        Aggregate agg3{42, 3.14, "text"};
        Aggregate agg4{.a = 42, .c = "text"}; // since C++20
    }

    SECTION("classes")
    {
        Class c1(142, 3.14, "text");
        Class c2{142, 3.14, "text"};
    }

    SECTION("arrays & containers")
    {
        int native_arr[] = {1, 2, 4, 5}; // intialization of aggragate
        std::vector<int> vec = {1, 2, 3, 4}; // initializer_list

        std::map<int, std::string> dict = { {1, "one"}, {2, "two"} };
    }
}

template <typename T>
class Container
{
    T* items_;
    size_t size_;
public:
    using iterator = T*;
    using const_iterator = const T*;

    Container(size_t size, const T& default_value)
    {
        size_ = size;
        items_ = new T[size_];
        std::fill_n(items_, size_, default_value); 
    }
    
    Container(std::initializer_list<T> lst)
    {
        size_ = lst.size();
        items_ = new T[size_]; 
        std::copy(lst.begin(), lst.end(), items_);
    }

    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;

    ~Container()
    {
        delete[] items_;
    }

    size_t size() const
    {
        return size_;
    }

    iterator begin()
    {
        return items_;
    }

    iterator end()
    {
        return items_ + size_;
    }

    const_iterator begin() const
    {
        return items_;
    }

    const_iterator end() const
    {
        return items_ + size_;
    }

    T& operator[](size_t index)
    {
        return *(items_ + index);
    }

    const T& operator[](size_t index) const
    {
        return items_[index];
    }
};

template <typename T, size_t N>
struct Array
{
    T items[N];
};

TEST_CASE("initializer_list")
{
    std::initializer_list<int> lst_1 = {1, 2, 3, 4, 5};
    auto lst_2 = {1, 2, 3, 4, 5};

    auto numbers = {1.0, 2.3, 3.44, static_cast<double>(9)};

    Container container = {1, 2, 3, 4, 5, 6, 7};
    REQUIRE(container[3] == 4);

    //Container other_container = container;

    std::vector expected = {1, 2, 3, 4, 5, 6, 7};
    REQUIRE(std::equal(container.begin(), container.end(), expected.begin(), expected.end()));

    Array<int, 10> arr = {1, 2, 3};
}

TEST_CASE("constructor with initializer")
{
    Container<int> c1 = {10, 20};
    REQUIRE(c1.size() == 2);

    Container<int> c2{10, 20};
    REQUIRE(c2.size() == 2);

    Container<int> c3(10, 20);
    REQUIRE(c3.size() == 10);

    std::vector<int> vec1 = {10, 20};
    REQUIRE(vec1 == std::vector{10, 20});

    std::vector<int> vec2(10, 20);
    REQUIRE(vec2 == std::vector{20, 20, 20, 20, 20, 20, 20, 20, 20, 20});
}

// class VariantContainer
// {
//     std::variant<int,std::string>* items_;
//     size_t size_;
// public:
//     VariantContainer(std::initializer_list<std::variant<int,std::string>> lst)
//     {
//         size_ = lst.size();
//         items_ = new std::variant<int,std::string>[size_]; 
//         std::copy(lst.begin(), lst.end(), items_);
//     }
// };

TEST_CASE("initializer_list variant")
{
    std::initializer_list<std::variant<int,std::string>> variant_1{1,"1"};
    std::initializer_list<std::variant<int,std::string>> variant_2 = {1,"1"};

    Container<std::variant<int, std::string>> container = {1, "2.2", 3, 4, 5, 6};
}