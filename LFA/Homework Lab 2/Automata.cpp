#include "Automata.hpp"

namespace Constants{

const std::string kSigma = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";

}

std::ostream& operator << (std::ostream& os, const Automata& fa)
{
    fa.PrintFA(os);
    return os;
}

std::istream& operator >> (std::istream& is, Automata& fa)
{
    fa.ReadFA(is);
    return is;
}