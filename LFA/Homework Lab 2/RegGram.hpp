#pragma once // Regular Grammar

#include "Automata.hpp"
#include "LNFA.hpp"

using namespace std;

class RegGram : public Automata
{
private:
    void PrintFA(ostream& os) const override;
    void ReadFA(istream& is) override;

    bool WordAccepted(const string& word) const override;

    UMap<string, vector<pair<char, string>>> _productions;
    string _start_state;

public:
    RegGram();

    RegGram(const string& start_state, 
            const UMap<string, vector<pair<char, string>>>& prods);
    
    DFA ToDFA() const;

    void BuildFromDFA(const DFA& dfa);
};