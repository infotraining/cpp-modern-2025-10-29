#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>

template <typename T>
class UniquePtr
{
    T* ptr_;

public:
    UniquePtr(nullptr_t)
        : ptr_{nullptr}
    {
    }

    UniquePtr()
        : ptr_{nullptr}
    { }

    explicit UniquePtr(T* ptr)
        : ptr_{ptr}
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // move constructor
    UniquePtr(UniquePtr&& other)
        : ptr_{other.ptr_}
    {
        other.ptr_ = nullptr;
    }

    // move assignment operator
    UniquePtr& operator=(UniquePtr&& other)
    {
        if (this != &other)
        {
            delete ptr_; // destroying previous state

            // moving the state from other to *this
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }

        return *this;
    }

    ~UniquePtr()
    {
        delete ptr_;
    }

    explicit operator bool() const
    {
        return ptr_ != nullptr;
    }

    T* get() const
    {
        return ptr_;
    }

    T* operator->() const
    {
        return ptr_;
    }

    T& operator*() const
    {
        return *ptr_;
    }
};

UniquePtr<Gadget> create_gadget()
{
    static int gen_id = 1000;
    const int id = ++gen_id;
    UniquePtr<Gadget> ptr_gadget{new Gadget(id, "Gadget#" + std::to_string(id))};
    return ptr_gadget;
}

TEST_CASE("move semantics - UniquePtr")
{
    UniquePtr<int> ptr_int = nullptr;

    UniquePtr<Gadget> pg1{new Gadget{1, "ipad"}};
    pg1->use();
    (*pg1).use();

    UniquePtr<Gadget> pg2 = std::move(pg1); // call move constructor
    REQUIRE(pg1.get() == nullptr);
    pg2->use();

    pg1 = UniquePtr<Gadget>(new Gadget{2, "smartwatch"});
    pg2 = std::move(pg1);
    pg2->use();

    {
        UniquePtr<Gadget> pg3 = create_gadget();
        pg3->use();

        std::vector<UniquePtr<Gadget>> gadgets;
        gadgets.push_back(std::move(pg3));  // xvalue
        gadgets.push_back(create_gadget()); // rvalue
        gadgets.push_back(create_gadget());
        gadgets.push_back(create_gadget());
        gadgets.push_back(create_gadget());

        for (const auto& pg : gadgets)
        {
            pg->use();
        }
        std::cout << "End of scope........\n";
    }

    std::cout << "After end of scope........\n";
}