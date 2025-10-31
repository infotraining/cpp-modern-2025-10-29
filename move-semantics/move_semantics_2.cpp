#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>

namespace StdExplain
{
    template <typename T>
    class UniquePtr
    {
        T* ptr_;

    public:
        UniquePtr(nullptr_t) noexcept
            : ptr_{nullptr}
        {
        }

        UniquePtr() noexcept
            : ptr_{nullptr}
        { }

        explicit UniquePtr(T* ptr) noexcept
            : ptr_{ptr}
        {
        }

        UniquePtr(const UniquePtr&) = delete;
        UniquePtr& operator=(const UniquePtr&) = delete;

        // move constructor
        UniquePtr(UniquePtr&& other) noexcept
            : ptr_{other.ptr_}
        {
            other.ptr_ = nullptr;
        }

        // move assignment operator
        UniquePtr& operator=(UniquePtr&& other) noexcept
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

        ~UniquePtr() noexcept
        {
            delete ptr_;
        }

        explicit operator bool() const noexcept
        {
            return ptr_ != nullptr;
        }

        T* get() const noexcept
        {
            return ptr_;
        }

        T* operator->() const noexcept
        {
            return ptr_;
        }

        T& operator*() const noexcept
        {
            return *ptr_;
        }
    };

    template <typename T>
    UniquePtr<T> make_unique()
    {
        return UniquePtr<T>(new T());
    }

    template <typename T, typename TArg1>
    UniquePtr<T> make_unique(TArg1&& arg1)
    {
        return UniquePtr<T>(new T(std::forward<TArg1>(arg1)));
    }

    template <typename T, typename TArg1, typename TArg2>
    UniquePtr<T> make_unique(TArg1&& arg1, TArg2&& arg2)
    {
        return UniquePtr<T>(new T(std::forward<TArg1>(arg1), std::forward<TArg2>(arg2)));
    }

    UniquePtr<Gadget> create_gadget()
    {
        static int gen_id = 1000;
        const int id = ++gen_id;
        UniquePtr<Gadget> ptr_gadget{new Gadget(id, "Gadget#" + std::to_string(id))};
        return ptr_gadget;
    }
} // namespace StdExplain

TEST_CASE("move semantics - UniquePtr")
{
    using StdExplain::UniquePtr;
    using StdExplain::create_gadget;


    UniquePtr<int> ptr_int = nullptr;

    // UniquePtr<Gadget> pg1{new Gadget{1, "ipad"}};
    auto pg1 = StdExplain::make_unique<Gadget>(1, "ipad");
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