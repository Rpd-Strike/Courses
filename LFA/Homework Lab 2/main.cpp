#include <bits/stdc++.h>

#include "LNFA.hpp"

using namespace std;

void ContinuoslyTestAutomata(const Automata& atm)
{
    string str;
    while (cin >> str) {
        cout << (atm.WordAccepted(str) ? "YES" : "NO") << '\n'; 
    }
}

int main()
{
    LNFA nfa;

    cin >> nfa;

    cout << "Initial LLLNFA: ===== \n" << nfa << "-----------------------\n";

    cout << "Normal NNNFA: ==== \n" << nfa.ToNFA() << "---------------------\n";

    cout << "Normal DDDFA: ===\n" << nfa.ToNFA().ToDFA() << "=-=----------=--=-=-=-=-==-\n";

    cout << "Minimized DFA: ===\n" << nfa.ToNFA().ToDFA().ToMinimalDFA() << "-------------=-=-=--=-=-=-=-==-\n";

    ContinuoslyTestAutomata(nfa);

    return 0;   
}