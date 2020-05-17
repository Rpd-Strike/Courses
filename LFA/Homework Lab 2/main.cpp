#include <bits/stdc++.h>

#include "LNFA.hpp"
#include "RegGram.hpp"

using namespace std;

void ContinuoslyTestAutomata(const Automata& atm)
{
    string str;
    while (cin >> str) {
        cout << (atm.WordAccepted(str) ? "YES" : "NO") << '\n'; 
    }
}

void TestLambdaNFA()
{
    LNFA nfa;

    cin >> nfa;

    cout << "Initial LLLNFA: ===== \n" << nfa << "-----------------------\n";

    cout << "Normal NNNFA: ==== \n" << nfa.ToNFA() << "---------------------\n";

    cout << "Normal DDDFA: ===\n" << nfa.ToNFA().ToDFA() << "=-=----------=--=-=-=-=-==-\n";

    cout << "Minimized DFA: ===\n" << nfa.ToNFA().ToDFA().ToMinimalDFA() << "-------------=-=-=--=-=-=-=-==-\n";

    ContinuoslyTestAutomata(nfa);
}

void TestRegGram()
{
    RegGram rg;
    cin >> rg;
    cout << "Regular grammar read:\n";
    cout << rg << '\n';
    cout << "======  Equivalent DFA: \n";
    cout << rg.ToDFA() << '\n';

    DFA dfa;
    cin >> dfa;
    rg.BuildFromDFA(dfa);
    cout << rg << '\n';
}

int main()
{
    TestRegGram();

    return 0;   
}


// ===== STRESSTEST

// #include <bits/stdc++.h>
 
// #include "LNFA.hpp"
 
// using namespace std;
 
// void ContinuoslyTestAutomata(const Automata& atm)
// {
//     string str;
//     while (cin >> str) {
//         cout << (atm.WordAccepted(str) ? "YES" : "NO") << '\n';
//     }
// }
 
 
// namespace MinimizationTester {
//     mt19937 rnd(time(0));
 
//     void tester(DFA x, int sigma)
//     {
//         DFA minim = x;
//         minim.Minimize();

//         cout << "Test:  \nStart DFA - " << x._total_states << "\n";
//         cout << "Minimezed DFA - " << minim._total_states << "\n";

//         for (int _ = 0; _ < 10000; _++) {
//             string s;
//             int l = rnd() % 8;
//             for (int i = 0; i < l; i++)
//                 s += 'a' + rnd() % sigma;
 
//             if (x.WordAccepted(s) ^ minim.WordAccepted(s)) {
//                 cerr << "\nError for " << s << '\n';
//                 cerr << "X.WordAccepted(" << s << ") = " << x.WordAccepted(s) << "\nBut minim.WordAccepted(" << s << ") = ";
//                 cerr << minim.WordAccepted(s) << "!!\n";
//                 cerr << "Initial DFA: " << x << '\n' << "Minimized DFA: " << minim << '\n';
//                 exit(0);
//             }
//         }
//     }
 
//     DFA make_random(int sigma)
//     {
//         int start_node = 0;
//         int l = rnd() % 15 + 1;
//         vector <int> end_nodes = { (int)(rnd() % l), (int)(rnd() % l) };
//         vector <unordered_map <char, int>> edges(l);
//         for (int i = 0; i < 2 * l * l; i++) {
//             char c = rnd() % sigma + 'a';
//             edges[rnd() % l][c] = rnd() % l;
//         }
 
//         return DFA(edges.size(), start_node, end_nodes, edges);
//     }
 
//     void Tester()
//     {
//         cerr << "Starting DFA-Minimization stress test ... ";
//         auto time_act = chrono::system_clock::now();
//         while (1) {
//             DFA r = make_random(8);
//             tester(r, 4);
//             auto now = chrono::system_clock::now();
//             if (chrono::duration_cast<chrono::seconds>(now - time_act).count() > 100) {
//                 cerr << "Pass\n";
//                 return;
//             }
//         }
//     }
// }
 
 
 
// int main()
// {
//     MinimizationTester::Tester();
//     LNFA nfa;
 
//     cin >> nfa;
 
//     cout << "Initial LLLNFA: ===== \n" << nfa << "-----------------------\n";
 
//     cout << "Normal NNNFA: ==== \n" << nfa.ToNFA() << "---------------------\n";
 
//     cout << "Normal DDDFA: ===\n" << nfa.ToNFA().ToDFA() << "=-=----------=--=-=-=-=-==-\n";
 
//     cout << "Minimized DFA: ===\n" << nfa.ToNFA().ToDFA().ToMinimalDFA() << "-------------=-=-=--=-=-=-=-==-\n";
 
//     ContinuoslyTestAutomata(nfa);
 
//     return 0;  
// }