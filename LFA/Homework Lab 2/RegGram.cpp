#include "RegGram.hpp"

bool RegGram::WordAccepted(const string& /* word */) const
{   
    throw runtime_error("Can not get accepted word in regular grammar");
}

void RegGram::PrintFA(ostream& os) const
{
    os << "Printing Regular Grammar:\n";
    os << "Nodes: " << _productions.size() << '\n';
    os << "Productions:\n";
    for (auto pp : _productions) {
        os << pp.first << " -> ";
        string between = "";
        for (auto el : pp.second) {
            os << between << el.first << el.second;
            between = " | ";
        }
        os << '\n';
    }
}

void RegGram::ReadFA(istream& is)
{
    try {
    // cout << "Reading Regular grammar:\n";
    // cout << "Start symbol: ";
    is >> _start_state;
    for (char c : _start_state) 
        if (not isupper(c))
            throw string("bad start state");
    // cout << "Nr. of productions: \n";
    int nr;
    is >> nr;  is.ignore();
    while (nr > 0) {
        --nr;
        string lhs;
        string line;
        getline(is, line);

        // cerr << line << "  HAHA\n";

        /// read lhs
        int ind = 0;
        while (ind < (int)line.size() && isupper(line[ind])) {
            lhs.push_back(line[ind]);
            ++ind;
        } 

        /// read  productions
        auto skip = [&]() -> void {
            while (ind < (int)line.size() && (!isalpha(line[ind]) && line[ind] != Constants::kEpsilon))
                ++ind;
        };
        auto getPereche = [&]() -> pair<char, string> {
            char ch;
            string to = "";
            if (ind >= (int)line.size())
                throw string("bad input");
            ch = line[ind];
            if (ch != Constants::kEpsilon && !islower(ch)) {
                if (isupper(ch)) {
                    to.push_back(ch);
                    ch = Constants::kEpsilon;
                }
                else{
                    throw string("bad input");
                }   
            }
            
            ++ind;
            while (ind < (int)line.size() && isupper(line[ind])) {
                to.push_back(line[ind]);
                ++ind;
            }
            return {ch, to};
        };

        while (ind < (int)line.size()) {
            skip();
            if (ind < (int)line.size()) {
                auto p = getPereche();
                _productions[lhs].push_back(p);
                // cerr << "prod: " << _productions[lhs].size() << " " << lhs << ' '
                //   << p.first << ' ' << p.second << '\n';
            }
        }
    }
    }
    catch (string s) {
        cerr << "Reading Regular grammar error: " << s << '\n';
        *this = RegGram();
    }
}

RegGram::RegGram()
{

}

RegGram::RegGram(const string& start_state, 
        const UMap<string, vector<pair<char, string>>>& prods)
{
    _start_state = start_state;
    _productions = prods;
}

DFA RegGram::ToDFA() const
{
    LNFA lnfa;
    UMap<string, int> ids;
    vector<UMap<char, USet<int>>> edges;

    auto calc_id = [&](string s) {
        if (ids.find(s) == ids.end()) {
            int id = ids.size();
            ids[s] = id;
            edges.push_back(UMap<char, USet<int>>());
        }
        return ids[s];
    };

    for (const auto& [neterm_from, edge_list] : _productions) {
        int lhs = calc_id(neterm_from);
        for (auto& [ch_tranz, string_neterminal] : edge_list) {
            edges[lhs][ch_tranz].insert( calc_id(string_neterminal) );
        }
    }

    vector<int> start_nodes = { calc_id(_start_state) };
    vector<int> end_states  = { calc_id("") };

    lnfa = LNFA(edges.size(), start_nodes, end_states, edges);

    cout << "------------ LambdaNFA\n";
    cout << lnfa << '\n';

    cout << "------------- NFA\n";
    cout << lnfa.ToNFA() << '\n';

    cout << "-------------- DFA\n";
    cout << lnfa.ToNFA().ToDFA() << '\n';

    return lnfa.ToNFA().ToDFA().ToMinimalDFA();
}

void RegGram::BuildFromDFA(const DFA& dfa)
{
    vector<string> neterminals;
    for (int i = 0; i < (int)dfa._total_states; ++i) {
        string neterm;
        int cod = i;
        do {
            neterm.push_back(char('A' + cod % 26));
            cod /= 26;
        } 
        while (cod > 0);

        neterminals.push_back(neterm);
    }
    _start_state = neterminals[dfa.GetStartNodeId()];
    _productions.clear();
    for (int i = 0; i < (int)dfa._total_states; ++i) {
        if (dfa._is_final.find(i) != dfa._is_final.end()) {
            if (dfa._is_final.at(i) == true)
                _productions[neterminals[i]].push_back({Constants::kEpsilon, ""});
        }
        for (const auto& vec : dfa._edges) {
            for (const auto& [ch_tran, to_node] : vec) {
                _productions[neterminals[i]].push_back({ch_tran, neterminals[to_node]});
            }
        }
    }
}