#ifndef AUTOMATA_HPP_
#define AUTOMATA_HPP_

#include <istream>
#include <ostream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace Constants
{
    const char kLambda = '#';
    const char kEpsilon = '#';
    extern const std::string kSigma; 
}

class Automata
{
private:
protected:
    template<typename A, typename B>
    using UMap = std::unordered_map<A, B>;
    template<typename A>
    using USet = std::unordered_set<A>;

    virtual void PrintFA(std::ostream& os) const = 0;
    virtual void ReadFA(std::istream& is) = 0;
public:
    virtual bool WordAccepted(const std::string& word) const = 0;

    friend std::ostream& operator << (std::ostream& os, const Automata& fa);
    friend std::istream& operator >> (std::istream& is, Automata& fa);
};

#endif // FINITE_AUTOMATA_HPP_