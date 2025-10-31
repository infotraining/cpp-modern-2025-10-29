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
        return *this;
    }

    /////////////////////////////////////////////////
    // move constructor - TODO
    Data(Data&& other) noexcept
        : name_{std::move(other.name_)} // noexcept
        , data_{other.data_}  // noexcept
        , size_{other.size_}  // noexcept
    {
        other.data_ = nullptr;  // noexcept
        other.size_ = 0;        // noexcept

        //std::cout << "Data(" << name_ << ": mv)\n";
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
            Data temp{std::move(other)}; // noexcept
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

    void swap(Data& other) noexcept
    {
        name_.swap(other.name_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    const std::string& name() const noexcept
    {
        return name_;
    }

    iterator begin() noexcept
    {
        return data_;
    }

    iterator end() noexcept
    {
        return data_ + size_;
    }

    const_iterator begin() const noexcept
    {
        return data_;
    }

    const_iterator end() const noexcept
    {
        return data_ + size_;
    }

    size_t size() const noexcept
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

    SECTION("move semantics by default")
    {
        Data row_1{"row_1", {1, 2, 3}};
        Data row_2{"row_2", {55, 26, 73}};
        Data row_3{"row_3", {16, 72, 38}};

        DataSet ds1{std::move(row_1), std::move(row_2), std::move(row_3)};

        std::cout << "---------------\n";

        DataSet target_ds1 = std::move(ds1);
    }
}

void foo(const std::string& str, const std::vector<int>& vec)
{
    std::cout << str << " " << vec.size() << "\n";
}

namespace SuperOptimalHandwired
{
    void foo_transfer(const std::string& str, const std::vector<int>& vec)
    { /* bounds to lvalue */              /* bounds to lvalue       */
        std::string str_local = str;      // lvalue copied to str_local
        std::vector<int> vec_local = vec; // lvalue copied to vec_local
    }

    void foo_transfer(std::string&& str, const std::vector<int>& vec)
    { /* bounds to rvalue */                    /* bounds to lvalue   */
        std::string str_local = std::move(str); // xvalue transferred to str_local
        std::vector<int> vec_local = vec;       // lvalue copied to vec_local
    }

    void foo_transfer(const std::string& str, std::vector<int>&& vec)
    { /* bounds to lvalue */                         /* bounds to rvalue */
        std::string str_local = str;                 // lvalue copied to str_local
        std::vector<int> vec_local = std::move(vec); // xvalue transferred to vec_local
    }

    void foo_transfer(std::string&& str, std::vector<int>&& vec)
    { /* bounds to rvalue */                         /* bounds to rvalue */
        std::string str_local = std::move(str);      // xvalue transferred to str_local
        std::vector<int> vec_local = std::move(vec); // xvalue transfered to vec_local
    }
} // namespace SuperOptimalHandwired

namespace SuperOptimal
{
    template<typename TArg1, typename TArg2>
    void foo_transfer(TArg1&& arg1, TArg2&& arg2)
    {
        std::string str_local = std::forward<TArg1>(arg1);      // lvalue copied to str_local
        std::vector<int> vec_local = std::forward<TArg2>(arg2); // lvalue copied to vec_local
    }

} // namespace SuperOptimalHandwired

TEST_CASE("foo_transfer with optimal overloads")
{
    using namespace std::literals;
    using SuperOptimalHandwired::foo_transfer;

    auto str = "text"s;
    std::vector vec = {1, 2, 3};

    foo_transfer(str, vec);                       // foo_transfer(const string&, const vector<int>&)
    foo_transfer("text"s, vec);                   // foo_transfer(std::string&&, const vector<int>&)
    foo_transfer(std::move(str), vec);            // foo_transfer(std::string&&, const vector<int>&)
    foo_transfer(str, std::vector{1, 2, 3});      // foo_transfer(const string&, std::vector<int>&&)
    foo_transfer(str, std::move(vec));            // foo_transfer(const string&, std::vector<int>&&)
    foo_transfer("text"s, std::vector{1, 2, 3});  // foo_transfer(std::string&&, std::vector<int>&&)
    foo_transfer(std::move(str), std::move(vec)); // foo_transfer(std::string&&, std::vector<int>&&)
}

inline namespace CompromiseImpl
{
    void foo_transfer(std::string str, std::vector<int> vec)
    {
        std::string str_local = std::move(str);
        std::vector<int> vec_local = std::move(vec);
    }
} // namespace CompromiseImpl

TEST_CASE("foo_transfer with value semantics - compromise implementation")
{
    using namespace std::literals;

    auto str = "text"s;
    std::vector vec = {1, 2, 3};

    foo_transfer(str, vec);                      // copies of both lvalues to arguments, then moves inside
    foo_transfer("text"s, vec);                  // RVO, copy of vector, then move inside
    foo_transfer(std::move(str), vec);           // move of string, copy of vector, then move inside
    foo_transfer(str, std::vector{1, 2, 3});     // copy of string, RVO, then moves inside
    foo_transfer("text"s, std::vector{1, 2, 3}); // RVO, RVO, then moves inside
}

namespace StdExplain
{
    template <typename T>
    std::remove_reference_t<T>&& move(T&& value)
    {
        return static_cast<std::remove_reference_t<T>&&>(value);
    }
}

TEST_CASE("move - how it works")
{
    std::string str = "text";
    std::string target_str = StdExplain::move(str); // static_cast<std::string&&>(str)
    REQUIRE(str.empty());

    int x = 42;
    int target_x = std::move(x); // primitive types are copied

    Data ds{"ds", {1, 2, 3, 4}};
    Data target_ds = std::move(ds); // delegetion to Data move semantics - move constructor called

    const Data cds{"cds", {1, 2, 3, 4}};
    Data target_cds = std::move(cds); // copy - underlying object is const - copy constructor called
}

struct Person
{
    int id{};
    std::string name;
    std::vector<std::string> friends;
    Data data{"data", {1, 2, 3}};

    Person() = default;

    Person(int id, std::string name, std::vector<std::string> friends)
        : id{id}
        , name{std::move(name)}
        , friends{std::move(friends)}
    { }

    // Person(const Person&) = default;
    // Person& operator=(const Person&) = default;
    // Person(Person&&) = default;
    // Person& operator=(Person&&) = default;

    // ~Person()
    // {
    //     std::cout << "~Person(id: " << id << ", name: '" << name << "')\n";
    // }

    void print() const
    {
        //...
    }
};

TEST_CASE("Person")
{
    Person default_p;
    Person p1{42, "Jan", {"Adam", "Ewa"}};

    Person p2 = p1; // cc
    std::cout << "------\n";
    Person p3 = std::move(p1); // mv
}

struct Matrix
{
    std::vector<std::vector<int>> data_;

public:
    Matrix(size_t dim)
        : data_(dim, std::vector<int>(dim))
    { }

    std::vector<int>& operator[](size_t index)
    {
        return data_[index];
    }

    const std::vector<int>& operator[](size_t index) const
    {
        return data_[index];
    }
};

namespace LegacyCode
{
    Matrix* create_matrix(size_t dim)
    {
        Matrix* m = new Matrix(1000);

        //...
        return m;
    }
} // namespace LegacyCode

namespace ModernCpp
{
    Matrix create_matrix(size_t dim)
    {
        Matrix m(1000);

        //...
        return m;
    }
} // namespace ModernCpp

TEST_CASE("Matrix")
{
    SECTION("Legacy code")
    {
        Matrix* m = LegacyCode::create_matrix(1000);

        // delete[] m; // UB

        // m[0][1] = 1000; // UB
    }

    SECTION("Modern Cpp")
    {
        {
            Matrix m = ModernCpp::create_matrix(1000);
            m[0][1] = 1000;
        }
    }
}

void foo_safe() noexcept
{
    std::vector<int> vec;

    vec.at(100) = 100;
    //throw std::runtime_error("Error#13");
}

TEST_CASE("noexcept")
{
    std::vector<Data> vec;

    vec.push_back(Data{"d1", {1, 2, 3}});
    std::cout << "------------\n";
    vec.push_back(Data{"d2", {1, 2, 3}});
    std::cout << "------------\n";
    vec.push_back(Data{"d3", {1, 2, 3}});
    std::cout << "------------\n";
    vec.push_back(Data{"d4", {1, 2, 3}});
    std::cout << "------------\n";
    vec.push_back(Data{"d5", {1, 2, 3}});

    for(int i = 6;  i <= 32; ++i)
    {
        std::cout << i << "------------\n";
        vec.push_back(Data{"d5", {1, 2, 3}});
    }
}

struct SafeBar
{
    void bar() noexcept
    {}
};

struct UnsafeBar
{
    void bar()
    {}
};

template <typename T>
void foo_maybe_safe(T obj) noexcept(noexcept(obj.bar()))
{
    obj.bar();
}

TEST_CASE("noexcept as operator")
{
    foo_maybe_safe(SafeBar{});
    foo_maybe_safe(UnsafeBar{});


}