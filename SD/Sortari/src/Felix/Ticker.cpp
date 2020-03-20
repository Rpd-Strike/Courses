#include "Felix/Ticker.hpp"

#include <iostream>

namespace Felix::Ticker
{
    using std::chrono::steady_clock;
    using std::chrono::system_clock;

    static const timeStamp timeProgramStarted = steady_clock::now();

    std::time_t timeStamp_to_time_t(timeStamp givenTime)
    {
        return system_clock::to_time_t(system_clock::now() +
            (givenTime - steady_clock::now()));
    }

    void testFunction()
    {
        std::cout << "Testing Ticker namespace\n";
        std::time_t classicLocalTime = timeStamp_to_time_t(timeProgramStarted);
        std::tm localTime = *localtime(&classicLocalTime);
        std::cout << "Local Time: " << asctime(&localTime);
        /// std::cout << "UTC Time: " << asctime(&UTCProgramStarted) << '\n';
        std::cout << "Finished testing Ticker\n";
    }

    double secondsSinceProgramStart()
    {
        timeStamp now = steady_clock::now();
        auto drr = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeProgramStarted).count();
        return drr / 1000.0;
    }

    Ticker::Ticker()
    {
        timeObjectCreated = timeLastReset = steady_clock::now();
    }

    void Ticker::resetClock()
    {
        timeLastReset = steady_clock::now();
    }

    double Ticker::secondsSinceLastClock() const
    {
        timeStamp now = steady_clock::now();
        auto drr = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeLastReset).count();
        return drr / 1000.0;
    }

    double Ticker::secondsSinceObjectCreation() const
    {
        timeStamp now = steady_clock::now();
        auto drr = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeObjectCreated).count();
        return drr / 1000.0;
    }
}