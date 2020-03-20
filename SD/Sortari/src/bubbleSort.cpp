#include "bubbleSort.hpp"

#include "Felix/Ticker.hpp"
#include "Felix/Colors.hpp"

namespace Sortari
{
    std::string BubbleSort(std::vector<int>& v)
    {
        using Felix::Colors::File::red;
        using Felix::Colors::File::reset;

        Felix::Ticker::Ticker bubbleClock;
        bool gata = 0;
        while (!gata) {
            if (bubbleClock.secondsSinceObjectCreation() > 10.0) {
                return red + "Timed out!" + reset;
            }
            gata = 1;
            for (int i = 0;  i + 1 < (int)v.size();  ++i) {
                if (v[i] > v[i + 1])
                    std::swap(v[i], v[i + 1]),
                    gata = 0;
            }
        }
        return "";
    }
}