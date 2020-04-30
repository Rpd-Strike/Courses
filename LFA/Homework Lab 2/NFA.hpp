#ifndef NFA_HPP_
#define NFA_HPP_

#include "Automata.hpp"
#include "DFA.hpp"

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <queue>
#include <set>

class NFA final : public Automata
{
public:
    typedef UMap<char, USet<int>> EdgeList;

protected:
    typedef std::vector<int> VI;

    template<typename T>
    using Set = std::set<T>;

    int                   _total_states;
    VI                    _initial_states;
    UMap<int, bool>       _is_final;
    std::vector<EdgeList> _edge;

    void PrintFA(std::ostream& os) const;
    void ReadFA(std::istream& is);

    USet<int> ToWhere(int node, char ch) const;
public:
    NFA(int nr_states = 1);

    NFA(int nr_st, VI init_st, VI final_st,
         std::vector<EdgeList> edges = std::vector<EdgeList>());

    bool WordAccepted(const std::string& word) const;

    DFA ToDFA() const;

    int AddState();

    void AddEdge(int from, int to, char ch);
};

#endif // NFA_HPP_