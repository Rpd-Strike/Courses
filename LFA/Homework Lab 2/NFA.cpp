#include "NFA.hpp"

void NFA::PrintFA(std::ostream& os) const
{   
    os << "Printing NFA:\n";
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
                os << (to_first ? "" : "    ") << ' ' << node << '\n',
                to_first = 0;
        }
        os << '\n';
    }
}

void NFA::ReadFA(std::istream& is)
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

NFA::NFA(int nr_states)
{
    if (nr_states < 1)
        throw std::runtime_error("Can't create NFA with less than 1 node!");
    _total_states = nr_states;
    _edge = std::vector<EdgeList>(_total_states);
}

NFA::NFA(int nr_st, VI init_st, VI final_st,
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
}

bool NFA::WordAccepted(const std::string& word) const 
{
    USet<int> active_nodes(_initial_states.begin(), _initial_states.end());
    for (char ch : word) {
        USet<int> next_nodes;
        for (int from : active_nodes)
            for (int to : ToWhere(from, ch))
                next_nodes.insert(to);
        active_nodes = next_nodes;
    }
    for (int node : active_nodes) {
        if (_is_final.find(node) != _is_final.end())
            if (_is_final.at(node))
                return true;
    }
    return false;
}

NFA::USet<int> NFA::ToWhere(int node, char ch) const
{
    USet<int> neighbours;
    if (_edge[node].find(ch) != _edge[node].end())
        neighbours = _edge[node].at(ch);
    return neighbours;
};



DFA NFA::ToDFA() const
{
    std::queue<Set<int>> queue;
    std::map<Set<int>, int> new_states;
    std::vector<DFA::EdgeList> new_edges;
    VI new_final_nodes;

    auto GetId = [&](const Set<int>& nodes) -> int
    {
        if (new_states.find(nodes) != new_states.end())
            return new_states[nodes];
        int id = new_states.size();
        new_states[nodes] = id;
        new_edges.push_back(DFA::EdgeList());
        queue.push(nodes);

        // std::cerr << "New node in DFA: ";
        // for (auto x : nodes) std::cerr << x << ' ';
        // std::cerr << '\n';

        return id;
    };

    auto IsFinal = [&](int node) -> bool
    {
        if (_is_final.find(node) != _is_final.end())
            if (_is_final.at(node))
                return true;
        return false;
    };

    Set<int> new_initial_set = 
        Set<int>({_initial_states.begin(), _initial_states.end()});
    int new_initial_node = GetId(new_initial_set);
 
    while (!queue.empty()) {
        auto nodes = queue.front();
        queue.pop();
        int id = GetId(nodes);

        for (char ch : Constants::kSigma) {
            Set<int> new_tranzition;
            for (int nod : nodes) {
                for (int to_node : ToWhere(nod, ch))
                    new_tranzition.insert(to_node);
                if (IsFinal(nod))
                    new_final_nodes.push_back(id);
            }
            if (!new_tranzition.empty()) {
                int new_id = GetId(new_tranzition);
                new_edges[GetId(nodes)][ch] = new_id;
            }
        }
    }

    return DFA(new_states.size(), new_initial_node, new_final_nodes, new_edges);
}

int NFA::AddState()
{
    ++_total_states;
    _edge.push_back(EdgeList());

    return _total_states - 1;
}

void NFA::AddEdge(int from, int to, char ch)
{
    if (from < 0 || from > _total_states || to < 0 || to > _total_states
        || Constants::kSigma.find(ch) == std::string::npos)
        throw std::string("Trying to add invalid Edge!");
    _edge[from][ch].insert(to);
}