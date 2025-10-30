#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string_view>

////////////////////////////////////////////////////////////////////////////
// Data - class with copy & move semantics (user provided implementation)

class Data
{
    std::string name_;
    int* data_{};
    size_t size_{};

public:
    using iterator = int*;
    using const_iterator = const int*;

    Data() = default;

    Data(std::string name, std::initializer_list<int> list)
        : name_{std::move(name)}
        , data_{new int[list.size()]}
        , size_{list.size()}
    {
        std::copy(list.begin(), list.end(), data_);

        std::cout << "Data(" << name_ << ")\n";
    }

    Data(const Data& other)
        : name_(other.name_)
        , data_{new int[other.size_]}
        , size_(other.size_)
    {
        std::copy(other.begin(), other.end(), data_);

        std::cout << "Data(" << name_ << ": cc)\n";
    }

    Data& operator=(const Data& other)
    {
        Data temp{other};
        swap(temp);
        std::cout << "Data=(" << name_ << ": cc)\n";

        // if (this != &other)
        // {
        //     delete[] data_;

        //     name_ = other.name_;
        //     size_ = other.size_;

        //     data_ = new int[size_];
        //     std::copy(other.data_, other.data_ + size_, data_);

        // }

        return *this;
    }

    /////////////////////////////////////////////////
    // move constructor - TODO
    Data(Data&& other)
        : name_{std::move(other.name_)}
        , data_{other.data_}
        , size_{other.size_}
    {
        other.data_ = nullptr;
        other.size_ = 0;

        std::cout << "Data(" << name_ << ": mv)\n";
    }

    /////////////////////////////////////////////////
    // move assignment - TODO
    // Data& operator=(Data&& other)
    // {
    //     if (this != &other)
    //     {
    //         name_ = std::move(other.name_);

    //         delete[] data_;
    //         data_ = other.data_;
    //         other.data_ = nullptr;

    //         size_ = other.size_;
    //         other.size_ = 0;
    //     }

    //     std::cout << "Data=(" << name_ << ": mv)\n";

    //     return *this;
    // }

    Data& operator=(Data&& other)
    {
        if (this != &other)
        {
            Data temp{std::move(other)};
            swap(temp);
        }

        std::cout << "Data=(" << name_ << ": move)\n";

        return *this;
    }

    ~Data()
    {
        if (data_)
            std::cout << "~Data(" << name_ << ")\n";
        else
            std::cout << "~Data(after move)\n";

        delete[] data_;
    }

    void swap(Data& other)
    {
        name_.swap(other.name_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    const std::string& name() const
    {
        return name_;
    }

    iterator begin()
    {
        return data_;
    }

    iterator end()
    {
        return data_ + size_;
    }

    const_iterator begin() const
    {
        return data_;
    }

    const_iterator end() const
    {
        return data_ + size_;
    }

    size_t size() const
    {
        return size_;
    }
};

Data create_data_set()
{
    Data ds{"data-set-one", {54, 6, 34, 235, 64356, 235, 23}};

    return ds;
}

template <typename TContainer>
void print(const TContainer& container, std::string_view prefix = "items")
{
    std::cout << prefix << " - [ ";
    for (const auto& item : container)
    {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

TEST_CASE("Data & move semantics")
{
    Data ds1{"ds1", {1, 2, 3, 4, 5}};
    print(ds1, "ds1");

    Data backup = ds1; // copy
    print(backup, "backup");

    Data target = std::move(ds1);
    print(target, "target");

    target = Data{"ds2", {3, 4, 535, 65, 765, 665}};
    print(target, "target");
}

class DataSet
{
    Data row_1;
    Data row_2;
    Data row_3;

public:
    DataSet(Data r1, Data r2, Data r3)
        : row_1(std::move(r1))
        , row_2(std::move(r2))
        , row_3(std::move(r3))
    {
    }

    // DataSet(const Data& r1, const Data& r2, const Data& r3)
    //     : row_1(r1)
    //     , row_2(r2)
    //     , row_3(r3)
    // {
    // }

    void print() const
    {
        ::print(row_1, row_1.name());
        ::print(row_2, row_2.name());
        ::print(row_3, row_3.name());
    }
};

TEST_CASE("DataSet")
{
    SECTION("passing lvalue")
    {
        Data row_1{"row_1", {1, 2, 3}};
        Data row_2{"row_2", {55, 26, 73}};
        Data row_3{"row_3", {16, 72, 38}};

        DataSet ds1{std::move(row_1), std::move(row_2), std::move(row_3)};

        ds1.print();
    }

    SECTION("passing rvalue")
    {
        DataSet ds1(Data{"row_1", {1, 2, 3}}, Data{"row_2", {65, 43, 2}}, Data{"row_3", {1, 2, 3, 4}});
        ds1.print();
    }
}

void foo(const std::string& str, const std::vector<int>& vec)
{
    std::cout << str << " " << vec.size() << "\n";
}

namespace SuperOptimal
{
    void foo_transfer(const std::string& str, const std::vector<int>& vec)
    {
        std::string str_local = str;
        std::vector<int> vec_local = vec;
    }

    void foo_transfer(std::string&& str, const std::vector<int>& vec)
    {
        std::string str_local = std::move(str);
        std::vector<int> vec_local = vec;
    }

    void foo_transfer(const std::string& str, std::vector<int>&& vec)
    {
        std::string str_local = str;
        std::vector<int> vec_local = std::move(vec);
    }

    void foo_transfer(std::string&& str, std::vector<int>&& vec)
    {
        std::string str_local = std::move(str);
        std::vector<int> vec_local = std::move(vec);
    }
} // namespace SuperOptimal

inline namespace CompromiseImpl
{
    void foo_transfer(std::string str, std::vector<int> vec)
    {
        std::string str_local = std::move(str);
        std::vector<int> vec_local = std::move(vec);
    }
}

TEST_CASE("foo with value semantics")
{
    using namespace std::literals;

    auto str = "text"s;
    std::vector vec = {1, 2, 3};

    foo(str, vec);

    foo_transfer(str, std::move(vec));

    foo_transfer("text"s, std::vector{1, 2, 3});
}
