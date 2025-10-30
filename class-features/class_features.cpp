#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Gadget
{
    uint32_t id_{};
    std::string name_{"not-set"};
    inline static uint32_t gen_id_{1000}; // since C++17

    static uint32_t gen_id()
    {
        return ++gen_id_;
    }

public:
    Gadget() = default;

    Gadget(uint32_t id, const std::string& name)
        : id_{id}
        , name_{name}
    { }

    Gadget(const std::string& name)
        : Gadget{gen_id(), name} // delegating constructor
    { }

    uint32_t id() const
    {
        return id_;
    }

    std::string name() const
    {
        return name_;
    }

    virtual void print() const
    {
        std::cout << "Gadget(id: " << id_ << ", name: " << name_ << ")\n";
    }
};

class SuperGadget : public Gadget
{
public:
    using Gadget::Gadget; // inheritance of all constructors
    SuperGadget(const std::string&) = delete;

    void print() const override
    {
        std::cout << "SuperGadget(id: " << id() << ", name: " << name() << ")\n";
    }
};

class HyperGadget : public SuperGadget
{
public:
    using SuperGadget::SuperGadget;

    void print() const override
    {
        std::cout << "HyperGadget(id: " << id() << ", name: " << name() << ")\n";
    }
};

void calculate(int value)
{
    std::cout << "calculate(int: " << value << ")\n";
}

void calculate(double) = delete;

TEST_CASE("delete calculate(double)")
{
    calculate(42);
    // calculate(3.1415);
    // calculate(3.1415f);
}

TEST_CASE("class features")
{
    Gadget g1{42, "ipad"};
    g1.print();

    Gadget g2;
    g2.print();

    SuperGadget sg1{665, "smartwatch"};

    Gadget* ref_g = &sg1;
    ref_g->print();

    HyperGadget sg2{333, "gadget-333"};
    ref_g = &sg2;
    ref_g->print();
}