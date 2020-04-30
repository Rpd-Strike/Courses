#include "DFA.hpp"

#include <tuple>

void DFA::PrintFA(std::ostream& os) const
{
    os << "Printing DFA:\n";
    os << "Number of states = " << _total_states << '\n';
    os << "Initial state = " << _initial_state << '\n';
    os << "Final state(s) = ";
    for (const auto& pp : _is_final) if (pp.second)
        os << pp.first << ' ';
    os << '\n';
    os << "Edges:\n";
    for (int from = 0; from < _total_states; ++from) {
        os << from << "  ";
        bool first = 1;
        for (const auto& ed : _edges[from])
            os << (first ? "" : "   ") << ed.first << ' ' << ed.second << '\n',
            first = 0;
        os << '\n';
    }
}

void DFA::ReadFA(std::istream& is)
{
    _total_states = _initial_state = -1;
    _edges.clear();
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
            _edges = std::vector<EdgeList>(_total_states);
            std::cerr << "Marime: " << _edges.size() << '\n';

            // init state
            cout << "Initial state = ";
            is >> _initial_state;
            if (_initial_state < 0 || _initial_state >= _total_states)
                throw std::string("Initial state not in given range");
            
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
            cout << "Successfull read of DFA  --  \n";
        }
        catch (std::string err) {
            cout << err << " ... Retry\n";
            read = false;
        }
    }
}

DFA::DFA(int nr_states /* = 1 */) : _total_states(nr_states)
{
    if (nr_states < 1)
        throw std::runtime_error("Can't create DFA with 0 or negative number of states");
    
    _initial_state = 0;
    _edges = std::vector<EdgeList>(nr_states);
}

DFA::DFA(int nr_st, int init_st, VI final_st, 
         std::vector<EdgeList> edges /* = std::vector<EdgeList>() */)
{
    if (nr_st < 1)
        throw std::runtime_error("Can't create DFA with 0 or negative number of states");

    _total_states = nr_st;
    _initial_state = init_st;
    for (auto x : final_st)
        _is_final[x] = 1;
    
    _edges = std::vector<EdgeList>(nr_st);
    for (size_t node = 0; node < edges.size(); ++node) {
        for (const auto& ed : edges[node]) {
            AddEdge(node, ed.second, ed.first);
        }
    }
}

bool DFA::WordAccepted(const std::string& word) const
{
    int node = _initial_state;
    for (auto ch : word) {
        if (_edges[node].find(ch) == _edges[node].end())
            return 0;
        node = _edges[node].at(ch);
    }
    if (_is_final.find(node) != _is_final.end())
        return _is_final.at(node);
    return 0;    
}

void DFA::AddEdge(int from, int to, char ch)
{
    assert(_total_states == (int)_edges.size());
    assert(0 <= from && from < (int)_edges.size());

    if (from < 0 || to < 0 || from >= _total_states || to >= _total_states 
            || Constants::kSigma.find(ch) == std::string::npos)
        throw std::string("Invalid edge provided!");
    
    if (_edges[from].find(ch) != _edges[from].end())
        throw std::runtime_error("Edge already existent in DFA");

    _edges[from][ch] = to;
}



void DFA::AlgRelabel()
{
    UMap<int, bool> to_delete, from_start, from_final;
    UMap<int, int> new_label;

    std::function<void(int)> dfs = [&](int node) {
        from_start[node] = 1;
        // std::cerr << "From source: " << node << '\n';
        for (const auto& ed : _edges[node]) {
            if (not from_start[ed.second])
                dfs(ed.second);
        }
    };
    std::function<void(int)> back_dfs = [&](int node) {
        from_final[node] = 1;
        // std::cerr << "From final: " << node << '\n';
        for (int from = 0; from < _total_states; ++from) {
            for (auto& pp : _edges[from]) {
                // std::cerr << "Edge: " << from << ' ' << pp.first << ' ' << pp.second << '\n';
                if (pp.second == node && from_final[from] == 0)
                    back_dfs(from);
            }
        }
    };
    /// find useless nodes
    dfs(_initial_state);
    for (const auto& pp : _is_final) 
        if (pp.second) // is final state
            back_dfs(pp.first);

    for (int i = 0; i < _total_states; ++i) {
        if (!from_start[i] || !from_final[i]) 
            to_delete[i] = true;
    }
    /// find new labels of nodes
    int cnt = 0;
    for (int node = 0; node < _total_states; ++node) {
        if (to_delete[node])
            ++cnt;
        if (not to_delete[node])
            new_label[node] = node - cnt;
    }

    /// update initial and final states;
    _initial_state = new_label[_initial_state];
    UMap<int, bool> new_finals;
    for (int node = 0; node < _total_states; ++node)
        if (not to_delete[node] && _is_final[node])
            new_finals[new_label[node]] = true;
    _is_final = new_finals;
    /// update the edges
    int nr_updated = 0;
    for (int node = 0; node < _total_states; ++node) {
        if (not to_delete[node])
            _edges[new_label[node]] = _edges[node],
            ++nr_updated; 
    }
    for (EdgeList& et : _edges) {
        EdgeList newEdge;
        for (auto& ed : et) {
            if (not to_delete[ed.second])
                newEdge[ed.first] = new_label[ed.second];
        }
        et = newEdge;
    }
    while ((int)_edges.size() > nr_updated)
        _edges.pop_back();
    
    /// check
    assert(nr_updated + cnt == _total_states);
    /// new number of states
    _total_states = nr_updated;    
}

int DFA::ToWhere(int node, char ch)
{
    if (node < 0 || node >= _total_states)
        throw std::runtime_error("Code tried to find out where\
                                  nonexistent node tranzition does");
    if (_edges[node].find(ch) == _edges[node].end())
        return -1;
    return _edges[node][ch];    
}

void DFA::Minimize()
{
    AlgRelabel();
    assert(_total_states == (int)_edges.size());

    /// generate classes
    int nr_classes = 0;
    std::vector<int> clasa(_total_states);
    UMap<int, std::vector<int>> the_class;
    for (int i = 0; i < _total_states; ++i)
        the_class[_is_final[i]].push_back(i);
    int old_nr_classes = the_class.size();
    if (the_class.size() == 1u) {
        if (the_class.begin()->first == 1) {
            the_class[0] = the_class[1];
            the_class.erase(1);
        }
    }
    nr_classes = the_class.size();
    assert(nr_classes > 0);
    assert(nr_classes == old_nr_classes);
    for (const auto& [id, cl_nodes] : the_class)
        for (int node : cl_nodes)
            clasa[node] = id;

    for (bool distinguish = true; distinguish; ) {
        distinguish = false;
        for (int id = 0; id < nr_classes; ++id) for (char ch : Constants::kSigma) {
            UMap<int, VI> splits;
            for (int node : the_class[id]) {
                int to_class = ToWhere(node, ch);
                if (to_class >= 0)
                    splits[to_class].push_back(node);
            }
            if (splits.size() > 1) {
                distinguish = true;
                int new_id = -1;
                for (const auto& [to_node, cl_nodes] : splits) {
                    [[maybe_unused]] const auto& _ = to_node;
                    /// thank you C++17 standard for not being complete ^^
                    new_id = (new_id == -1 ? id : nr_classes++);
                    the_class[new_id] = cl_nodes;
                    for (int node : cl_nodes)
                        clasa[node] = new_id;
                }
            }
        }
    }
    using std::cout;
    // std::cerr << "New classes: ";
    // for (int i = 0;  i < _total_states;  ++i)
    //     std::cerr << i << " - " << clasa[i] << '\n';
    
    int new_total_states = nr_classes;
    int new_start_node = clasa[_initial_state];

    UMap<int, bool> new_is_final;
    for (int id = 0; id < nr_classes; ++id)
        assert(!the_class[id].empty()),
        new_is_final[id] = _is_final[ the_class[id][0] ];
    
    std::vector<EdgeList> new_edges(nr_classes);
    for (int id = 0; id < nr_classes; ++id) {
        for (const auto& [ch, node_to] : _edges[ the_class[id][0] ]) {
            if (new_edges[id].find(ch) != new_edges[id].end())
                assert(new_edges[id][ch] == clasa[node_to]);
            new_edges[id][ch] = clasa[node_to];
        }
    }

    _total_states = new_total_states;
    _initial_state = new_start_node;
    _is_final = new_is_final;
    _edges = new_edges;
}


DFA DFA::ToMinimalDFA() const
{
    DFA copy(*this);
    copy.Minimize();
    return copy;
}

int DFA::AddState()
{
    ++_total_states;
    _edges.push_back(EdgeList());

    return _total_states - 1;
}

void DFA::SetFinalState(int id)
{
    if (id < 0 || id > _total_states)
        throw std::runtime_error("Setting non existent state in DFA");

    _is_final[id] = 1;
}

void DFA::SetNotFinalState(int id)
{
    if (id < 0 || id > _total_states)
        throw std::runtime_error("Setting non existent state in DFA");
        
    _is_final[id] = 0;
}