#ifndef __RANDOM_HPP__
#define __RANDOM_HPP__

#include <random>

namespace Felix::Random
{
    static std::random_device Felix_rd;
    static std::mt19937 Felix_gen(Felix_rd());
    
    int generateRandom(int from, int to);
}

#endif