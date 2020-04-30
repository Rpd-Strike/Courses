#ifndef DFA_HPP_
#define DFA_HPP_

#include "Automata.hpp"

#include <cassert>
#include <vector>
#include <unordered_map>
#include <functional>
#include <tuple>

class DFA final : public Automata
{
public:
    typedef  UMap<char, int>   EdgeList;

protected:
    typedef  std::vector<int>  VI;
    
    int                   _total_states;
    int                   _initial_state;
    UMap<int, bool>       _is_final;
    std::vector<EdgeList> _edges;

    void PrintFA(std::ostream& os) const;
    void ReadFA(std::istream& is);

    void AlgRelabel();

    int  ToWhere(int node, char ch);
public:
    DFA(int nr_states = 1);

    DFA(int nr_st, int init_st, VI final_st, 
        std::vector<EdgeList> edges = std::vector<EdgeList>());

    void AddEdge(int from, int to, char ch);

    void Minimize(); 

    bool WordAccepted(const std::string& word) const;

    DFA ToMinimalDFA() const;
    
    
    /// Kind of useless but why not

    int AddState(); 

    void SetFinalState(int id); 

    void SetNotFinalState(int id);     
};

#endif // DFA_HPP_