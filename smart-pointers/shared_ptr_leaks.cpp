#include <iostream>
#include <memory>
#include <string>
#include <catch2/catch_test_macros.hpp>

class Human : public std::enable_shared_from_this<Human>
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

    void set_partner(std::weak_ptr<Human> partner)
    {
        partner_ = partner;

        std::shared_ptr living_partner = partner.lock();

        if (living_partner->partner_.lock() != shared_from_this())
            // partner->set_partner(std::shared_ptr<Human>(this)); // Unsafe
            living_partner->set_partner(shared_from_this());
    }

    void description() const
    {
        std::cout << "My name is " << name_;

        if (std::shared_ptr<Human> living_partner = partner_.lock(); living_partner)
        {
            std::cout << " and my partner is " << living_partner->name_ << std::endl;			
        }

		std::cout << std::endl;
    }

private:
    std::weak_ptr<Human> partner_;
    std::string name_;
};

TEST_CASE("memory_leak_demo")
{
    // RC person1 == 1
    auto person1 = std::make_shared<Human>("Adam");

    // RC person2 == 1
    auto person2 = std::make_shared<Human>("Ewa");

    // RC person2 == 1
    person1->set_partner(person2);

    // RC person1 == 1
    // person2->set_partner(person1);

    REQUIRE(person1.use_count() == 1);
    REQUIRE(person2.use_count() == 1);

    person1->description();
    person2->description();
}
