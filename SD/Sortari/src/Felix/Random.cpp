#include "Felix/Random.hpp"

namespace Felix::Random
{
    int generateRandom(int from, int to)
    {
        std::uniform_int_distribution<int> distribution(from, to);
        return distribution(Felix_gen);
    }
}