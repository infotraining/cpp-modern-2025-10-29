#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>

// #define MSVC

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

////////////////////////////////////////////////////////
///  PERFECT FORWARDING

void have_fun(Gadget& g)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    g.use();
}

void have_fun(const Gadget& cg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    cg.use();
}

void have_fun(Gadget&& g)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    g.use();
}

namespace HandwiredForwarding
{
    void use(Gadget& g)
    {
        have_fun(g);
    }

    void use(const Gadget& g)
    {
        have_fun(g);
    }

    void use(Gadget&& g)
    {
        have_fun(std::move(g));
    }
} // namespace HandwiredForwarding

namespace PerfectForwarding
{
    namespace StdExplain
    {
        // template <typename T>
        // decltype(auto) forward(T&& value)
        // {
        //     if constexpr (std::is_lvalue_reference_v<T>)
        //         return value;
        //     else
        //         return std::move(value);
        // }
    } // namespace StdExplain

    template <typename TGadget>
    void use(TGadget&& g)
    {
        have_fun(std::forward<TGadget>(g));
    }
} // namespace PerfectForwarding

TEST_CASE("custom forwarding")
{
    Gadget g{1, "gadget"};
    const Gadget cg{2, "const-gadget"};

    using namespace PerfectForwarding;

    use(g);
    use(cg);
    use(Gadget{3, "temp-gadget"});
}

template <typename T>
void collapse(T& obj)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

TEST_CASE("reference collapsing")
{
    int x = 42;
    collapse<int>(x);
    collapse<int&>(x);
    collapse<int&&>(x);
}

TEST_CASE("std::vector & forwarding")
{
    std::vector<Gadget> gadgets;

    Gadget g{42, "ipad"};
    gadgets.push_back(g);

    gadgets.push_back(std::move(g));
    gadgets.push_back(Gadget{665, "smartwatch"});

    gadgets.emplace_back(667, "big watch");
}