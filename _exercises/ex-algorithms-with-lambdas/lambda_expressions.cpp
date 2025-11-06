#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using ToDo = void;

class TemperatureMonitor;

using TempChangedCallback = std::function<void(TemperatureMonitor&, int)>; 

// Implement the TemperatureMonitor class with observers support
class TemperatureMonitor
{
    std::string id_;
    std::vector<TempChangedCallback> observers_;
    
public:
    explicit TemperatureMonitor(std::string id)
        : id_(std::move(id))
    { }

    virtual ~TemperatureMonitor() = default;

    double read_temperature()
    {
        double temperature = current_temperature();

        notify_observers(temperature);
        
        return temperature;
    }

    std::string id() const
    {
        return id_;
    }

    void add_observer(TempChangedCallback observer)
    {
        observers_.push_back(std::move(observer));
    }

protected:
    void notify_observers(double current_temp)
    {
        for(auto& observer : observers_)
        {
            observer(*this, current_temp);
        }
    }

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

struct Fan
{
    void on()
    {
        std::cout << "Fan is on...\n";
    }

    void off()
    {
        std::cout << "Fan is off...\n";
    }
};

TEST_CASE("lambda expressions - logging temperature monitor")
{
    // Uncomment the code below
    Logger logger;
    Fan fan;

    FakeTemperatureMonitor monitor("TM-1", {60, 30, 28});
    
    monitor.add_observer([&logger](TemperatureMonitor& temp_monitor, int temp) {
         logger.log("Temperature " + temp_monitor.id() + ": " + std::to_string(temp) + "°C");
    });
    monitor.add_observer([&fan](TemperatureMonitor& temp_monitor, int temp) {
        if (temp > 50)
            fan.on();
        else if (temp < 20)
        {
            fan.off();
        } 
    });

    monitor.read_temperature();

    CHECK(logger.logs().size() == 1);
    CHECK(logger.logs().at(0) == "Temperature TM-1: 60°C");
}