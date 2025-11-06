#include "utils.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <thread>

// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource

using namespace std;
using namespace Utils;

namespace LegacyCode
{
    // forward declarations
    Gadget* get_gadget(const std::string& name);
    void use(Gadget* g);
    void use_gadget(Gadget* g);

    // definitions
    Gadget* get_gadget(const std::string& name)
    {
        static int id = 665;
        return new Gadget(++id, name);
    }

    void use(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";

        delete g;
    }

    void use_gadget(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }

    Gadget* get_gadgets(size_t size)
    {
        Gadget* tab = new Gadget[size];
        for (size_t i = 0; i < size; ++i)
        {
            tab[i] = Gadget(i, "Gadget#" + std::to_string(i));
        }

        return tab;
    }
} // namespace LegacyCode

TEST_CASE("Legacy hell with dynamic memory")
{
    using namespace LegacyCode;

    // Uncomment the following code
    {
        Gadget* g = get_gadget("ipad");

        use_gadget(g);
    } // memory leak

    {
        use_gadget(get_gadget("ipad"));
    } // memory leak

    {
        Gadget* g = get_gadget("ipad");

        use(g);

        // delete g; // UB - double delete

        // std::cout << g->name() << std::endl; // UB - use after delete
    }
}

namespace ModernCpp
{
    std::unique_ptr<Gadget> get_gadget(const std::string& name);
    void use(std::unique_ptr<Gadget> g);
    void use_gadget(Gadget* g);

    // definitions
    std::unique_ptr<Gadget> get_gadget(const std::string& name)
    {
        static int id = 665;
        return std::make_unique<Gadget>(++id, name);
    }

    void use(std::unique_ptr<Gadget> g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }

    void use_gadget(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }
} // namespace ModernCpp

TEST_CASE("Safer code with smart pointers")
{
    using namespace ModernCpp;

    {
        std::unique_ptr<Gadget> g = get_gadget("ipad");

        use_gadget(g.get());
    }

    {
        use(get_gadget("ipad"));
    }

    {
        std::unique_ptr<Gadget> g = get_gadget("ipad");

        use(std::move(g));
    }
}

TEST_CASE("Working with legacy code")
{
    std::unique_ptr<Gadget> g(LegacyCode::get_gadget("smartwatch"));

    LegacyCode::use_gadget(g.get());

    LegacyCode::use(g.release());

    SECTION("working with arrays from legacy code")
    {
        std::unique_ptr<Gadget[]> gadgets(LegacyCode::get_gadgets(10)); // calls delete[] in destructor
        std::cout << gadgets[5].name() << "\n";
    }
}

template <typename T>
using unique_for_malloc = std::unique_ptr<T, void (*)(void*)>;

int* create_with_malloc()
{
    void* raw_mem = malloc(sizeof(int));
    return reinterpret_cast<int*>(raw_mem);
}

TEST_CASE("custom deallocators")
{
    // fopen / fclose
    SECTION("Legacy code")
    {
        FILE* f = fopen("data0.txt", "w");

        if (!f)
        {
            std::cout << "File not opened!!!\n";
        }
        else
        {
            fprintf(f, "abc");
            fclose(f);
        }
    }

    SECTION("unique_ptr with custom dealloc")
    {
        std::unique_ptr<FILE, int (*)(FILE*)> safe_f(fopen("data1.txt", "w"), &fclose);

        if (safe_f)
        {
            fprintf(safe_f.get(), "abc");
        }

        auto file_closer = [](FILE* f) {
            std::cout << "File is closed!!!\n";
            fclose(f);
        };

        std::unique_ptr<FILE, decltype(file_closer)> another_safe_f(fopen("data2.txt", "w"), file_closer);

        if (another_safe_f)
        {
            fprintf(another_safe_f.get(), "abc");
        }
    }

    SECTION("malloc")
    {
        unique_for_malloc<int> int_with_malloc(create_with_malloc(), &free);

        *int_with_malloc = 42;

        std::cout << "value: " << *int_with_malloc << "\n";
    }
}

TEST_CASE("shared_ptrs with threads")
{
    std::thread thd_1;
    std::thread thd_2;

    {
        auto dict = std::make_shared<const std::map<int, std::string>>(
            std::initializer_list<std::pair<const int, std::string>>{ {1, "one"}, {2, "two"} });

        thd_1 = std::thread([dict] {
            std::cout << "Thread#1 started..." << std::endl;
            std::this_thread::sleep_for(1s);

            auto pos = dict->find(1);
            if (pos != dict->end())
            {
                std::cout << "item: " << pos->second << "\n";
            }
        });

        thd_2 = std::thread([dict] {
            std::cout << "Thread#2 started..." << std::endl;
            std::this_thread::sleep_for(12s);

            auto pos = dict->find(2);
            if (pos != dict->end())
            {
                std::cout << "item: " << pos->second << "\n";
            }
        });
    }

    thd_1.join();
    thd_2.join();
}
