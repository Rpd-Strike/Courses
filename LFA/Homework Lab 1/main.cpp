#include <iostream>
#include <fstream>
#include "Automat.hpp"

std::ifstream in("input.txt");
std::ofstream out("output.txt");

int main()
{
    Automat DFA;
    in >> DFA;
    int queries;
    in >> queries;
    while (queries--) {
        std::string word;
        in >> word;
        bool accepted = DFA.isWordAccepted(word);
        out << accepted << " " << word << "       ";
        out << (accepted ? "\"Accepted\"" : "\"Denied\"") << '\n';
    }
    out << '\n';
    DFA.printShortestLengthWords(30, out);
    return 0;
}