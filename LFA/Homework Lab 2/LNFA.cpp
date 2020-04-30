#include "LNFA.hpp"

void LNFA::PrintFA(std::ostream& os) const
{
    os << "Printing LNFA:\n";
    os << "Number of states = " << _total_states << '\n';
    os << "Initial state(s) = ";
    for (auto node : _initial_states)
        os << node << ' ';
    os << '\n';
    os << "Final state(s) = ";
    for (const auto& pp : _is_final) if (pp.second)
        os << pp.first << ' ';
    os << '\n';
    os << "Edges:\n";
    for (int from = 0; from < _total_states; ++from) {
        os << from << "  ";
        bool first = 1;
        for (const auto& [ch, to_nodes] : _edge[from]) {
            os << (first ? "" : "   ") << ch;
            first = 0;
            bool to_first = 1;
            for (int node : to_nodes)
                os << (to_first ? "" : "     ") << ' ' << node << '\n',
                to_first = 0;
        }
        os << '\n';
    }
}

void LNFA::ReadFA(std::istream& is)
{
    _total_states = -1;
    _initial_states.clear();
    _edge.clear();
    _is_final.clear();

    using std::cout;
    bool read = false;
    while (not read) {
        read = true;
        try {
            /// nr states
            cout << "Number of states = ";
            is >> _total_states;
            if (_total_states < 1)
                throw std::string("Number of states can't be negative!");
            _edge = std::vector<EdgeList>(_total_states);
            // std::cerr << "Marime: " << _edge.size() << '\n';

            // init state
            cout << "Number of initial states = ";
            int nr_init;
            is >> nr_init;
            if (nr_init < 0)
                throw std::string("Can not have negative number of initial states");
            while (nr_init-- > 0) {
                int state;
                is >> state;
                if (state < 0 || state > _total_states)
                    throw std::string("Given initial state out of bounds");
                _initial_states.push_back(state);
            }
            
            /// final states
            _is_final.clear();
            int nr_fin_states;
            cout << "Number of final states = ";
            is >> nr_fin_states;
            if (nr_fin_states < 0)
                throw std::string("Number of final states can't be negative!");
            cout << "Space enumerate the final states: ";
            while (nr_fin_states-- > 0) {
                int state;
                is >> state;
                if (state < 0 || state > _total_states)
                    throw std::string("Final state not in given range");
                _is_final[state] = true;
            }
            
            /// edges
            int nr_edges;
            cout << "Number of edges = ";
            is >> nr_edges;
            if (nr_edges < 0)
                throw std::string("Number of edges can't be negative!");
            while (nr_edges-- > 0) {
                int from, to; char ch;
                is >> from >> to >> ch;
                // std::cerr << "Read: " << from << ' ' << to << ' ' << ch << '\n';
                AddEdge(from, to, ch);
            }
            cout << "Succesful read of NFA  --  \n";
        }
        catch (std::string err) {
            cout << err << " ... Retry\n";
            read = false;
        }
    }
}

LNFA::LNFA(int nr_states)
{
    if (nr_states < 1)
        throw std::runtime_error("Can't create NFA with less than 1 node!");
    _total_states = nr_states;
    _edge = std::vector<EdgeList>(_total_states);
}  // TO DO

LNFA::LNFA(int nr_st, VI init_st, VI final_st,
        std::vector<EdgeList> edges)
{
    if (nr_st < 1)
        throw std::runtime_error("Can't create NFA with less than 1 node!");
    _total_states = nr_st;
    _edge = edges;
    _initial_states = init_st;
    _is_final.clear();
    for (int node : final_st)
        _is_final[node] = 1;
}  // TO DO

LNFA::USet<int> LNFA::ToWhere(int node, char ch) const
{
    USet<int> neighbours;
    if (_edge[node].find(ch) != _edge[node].end())
        neighbours = _edge[node].at(ch);
    return neighbours;
};

LNFA::USet<int> LNFA::ToLambda(int node) const
{
    USet<int> visited;

    std::function<void(int)> dfs = [&](int node) -> void
    {
        visited.insert(node);
        if (_edge[node].find(Constants::kLambda) != _edge[node].end())
            for (int to_node : _edge[node].at(Constants::kLambda))
                if (visited.find(to_node) == visited.end())
                    dfs(to_node);
    };

    dfs(node);

    return visited;
}

bool LNFA::WordAccepted(const std::string& word) const
{
    auto IsFinal = [&](int node) -> bool
    {
        if (_is_final.find(node) != _is_final.end())
            if (_is_final.at(node))
                return true;
        return false;
    };

    USet<int> active_nodes(_initial_states.begin(), _initial_states.end());
    for (char ch : word) {
        USet<int> next_nodes;
        /// Go to lambda edges
        for (int nod : active_nodes) {
            for (int to_nod : ToLambda(nod)) 
                next_nodes.insert(to_nod);
        }
        active_nodes = next_nodes;
        next_nodes.clear();
        for (int nod : active_nodes) {
            for (int to_nod : ToWhere(nod, ch))
                next_nodes.insert(to_nod);
        }
        active_nodes = next_nodes;
    }
    for (int nod : active_nodes)
        for (int to_nod : ToLambda(nod))
            if (IsFinal(to_nod))
                return true;
    return false;
}  // TO DO

NFA LNFA::ToNFA() const
{
    assert(_total_states == (int)_edge.size());

    auto IsFinal = [&](int node) -> bool
    {
        if (_is_final.find(node) != _is_final.end())
            if (_is_final.at(node))
                return true;
        return false;
    };

    std::vector<NFA::EdgeList> new_edges(_total_states);
    VI new_is_final;
    /// Calculate new transitions skipping any possible lambda
    for (int node = 0;  node < _total_states;  ++node)
        for (int to_lambda : ToLambda(node))
            for (const auto& [ch, to_nodes] : _edge[to_lambda])
                if (ch != Constants::kLambda)
                    for (int to_node : to_nodes)
                        new_edges[node][ch].insert(to_node);
    /// Get vector<int> for final states
    for (int node = 0; node < _total_states; ++node) 
        if (IsFinal(node))
            new_is_final.push_back(node);
    
    return NFA(_total_states, _initial_states, new_is_final, new_edges);
}  // TO DO

int LNFA::AddState()
{
    ++_total_states;
    _edge.push_back(EdgeList());

    return _total_states - 1;
}

void LNFA::AddEdge(int from, int to, char ch)
{
    if (from < 0 || from > _total_states || to < 0 || to > _total_states
  || (ch != Constants::kLambda && Constants::kSigma.find(ch) == std::string::npos))
        throw std::string("Trying to add invalid Edge!");

    _edge[from][ch].insert(to);
}