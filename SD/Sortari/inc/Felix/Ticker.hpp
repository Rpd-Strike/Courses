#ifndef __TICKER_HPP__
#define __TICKER_HPP__

#include <chrono>

namespace Felix::Ticker
{
    using timeStamp = std::chrono::time_point<std::chrono::steady_clock>;

    void testFunction();

    double secondsSinceProgramStart();

    class Ticker
    {
    private:
        timeStamp timeObjectCreated;
        timeStamp timeLastReset;
    public:
        Ticker();
        void resetClock();
        double secondsSinceLastClock() const;
        double secondsSinceObjectCreation() const;
    };
}

#endif