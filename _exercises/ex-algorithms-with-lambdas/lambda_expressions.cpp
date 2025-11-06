#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using ToDo = void;

using CallbackFunction = ToDo; 


// Implement the TemperatureMonitor class with observers support
class TemperatureMonitor
{
    std::string id_;
    // TODO: add observers container
    
public:
    explicit TemperatureMonitor(std::string id)
        : id_(std::move(id))
    { }

    virtual ~TemperatureMonitor() = default;

    double read_temperature()
    {
        double temperature = current_temperature();

        // notify_observers(temperature); // uncomment when implemented
        
        return temperature;
    }

    std::string id() const
    {
        return id_;
    }

    // TODO: add add_observer method

protected:
    // TODO: implement notify_observers method

    virtual double current_temperature() = 0;
};

class FakeTemperatureMonitor : public TemperatureMonitor
{
    std::vector<int> temperatures_;
    size_t index_ = 0;

public:
    FakeTemperatureMonitor(std::string id, std::vector<int> temperatures)
        : TemperatureMonitor(std::move(id))
        , temperatures_(std::move(temperatures))
    { }

protected:
    double current_temperature() override
    {
        if (index_ < temperatures_.size())
        {
            return temperatures_[index_++];
        }

        return 0.0;
    }
};

class Logger
{
    std::vector<std::string> logs_;

public:
    Logger() = default;

    void log(const std::string& message)
    {
        logs_.push_back(message);
    }

    const std::vector<std::string>& logs() const
    {
        return logs_;
    }
};

auto todo = [](auto&&...) { throw std::logic_error("Not implemented"); };

TEST_CASE("lambda expressions - logging temperature monitor")
{
    // Uncomment the code below
    // Logger logger;

    // FakeTemperatureMonitor monitor("TM-1", {25, 30, 28});
    // monitor.add_observer(todo);

    // monitor.read_temperature();

    // CHECK(logger.logs().size() == 1);
    // CHECK(logger.logs().at(0) == "Temperature TM-1: 25°C");
}