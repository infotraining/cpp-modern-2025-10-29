#include <iostream>
#include <memory>
#include <string>
#include <catch2/catch_test_macros.hpp>

class Human
{
public:
    Human(std::string name)
        : name_(std::move(name))
    {
        std::cout << "Ctor Human(" << name_ << ")" << std::endl;
    }

    ~Human()
    {
        std::cout << "Dtor ~Human(" << name_ << ")" << std::endl;
    }

    void set_partner(std::shared_ptr<Human> partner)
    {
        partner_ = partner;
    }

    void description() const
    {
        std::cout << "My name is " << name_;

        if (partner_)
        {
            std::cout << " and my partner is " << partner_->name_ << std::endl;			
        }

		std::cout << std::endl;
    }

private:
    std::shared_ptr<Human> partner_;
    std::string name_;
};

TEST_CASE("memory_leak_demo")
{
    // RC husband == 1
    auto person1 = std::make_shared<Human>("Adam");

    // RC wife == 1
    auto person2 = std::make_shared<Human>("Ewa");

    // RC wife ==2
    person1->set_partner(person2);

    // RC husband == 2
    person2->set_partner(person1);

    person1->description();
}
