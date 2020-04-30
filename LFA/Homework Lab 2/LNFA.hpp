#ifndef LNFA_HPP_
#define LNFA_HPP_

#include "Automata.hpp"
#include "NFA.hpp"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

class LNFA final : public Automata
{
public:
    typedef std::vector<int> VI;

protected:
    typedef std::unordered_map<char, std::unordered_set<int>> EdgeList;

    int                   _total_states;
    VI                    _initial_states;
    UMap<int, bool>       _is_final;
    std::vector<EdgeList> _edge;

    void PrintFA(std::ostream& os) const;
    void ReadFA(std::istream& is);

    USet<int> ToWhere(int node, char ch) const;

    USet<int> ToLambda(int node) const;

public:
    LNFA(int nr_states = 1);

    LNFA(int nr_st, VI init_st, VI final_st,
         std::vector<EdgeList> edges = std::vector<EdgeList>());

    bool WordAccepted(const std::string& word) const;

    NFA ToNFA() const;

    int AddState();

    void AddEdge(int from, int to, char ch);
};

#endif // LNFA_HPP_