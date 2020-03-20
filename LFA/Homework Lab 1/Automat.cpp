#include "Automat.hpp"

Automat::Automat() : nr_states(-1) {}

Automat::Automat(int nr_nodes) : states(std::vector<edgeList>(nr_nodes)) {}

std::istream& operator >> (std::istream& ifs, Automat& Atm)
{
    int nr_edges, nr_final_states;
    ifs >> Atm.nr_states >> nr_edges;
    Atm.states = std::vector<Automat::edgeList>(Atm.nr_states);
    Atm.final_states.clear();
    for (int i = 0;  i < nr_edges;  ++i) {
        int x, y;
        char c;
        ifs >> x >> y >> c;
        Atm.states[x][c].push_back(y);
    }
    ifs >> Atm.start_state;
    ifs >> nr_final_states;
    for (int i = 0;  i < nr_final_states;  ++i) {
        int x;
        ifs >> x;
        Atm.final_states[x] = 1;
    }
    return ifs;
}

bool Automat::isWordAccepted(const std::string& str) const
{
    std::unordered_set<int> current_states;
    current_states.insert(start_state);
    for (const char& ch : str) {
        std::unordered_set<int> next_states;
        for (const int& curr_node : current_states) {
            if (states.at(curr_node).find(ch) == states.at(curr_node).end())
                continue;
            for (const int& next_node : states.at(curr_node).at(ch)) {
                next_states.insert(next_node);
            }
        }
        current_states = next_states;
        if (current_states.empty())
            break;
    }
    for (const int& id : current_states)
        if (final_states.find(id) != final_states.end())
            return true;
    return false;
}

std::string calc_string(int poz, std::vector<int>& de_unde, std::vector<char>& last_edge)
{
    std::string sol;
    while( poz > 0 ) {
        sol.push_back(last_edge[poz]);
        poz = de_unde[poz];
    }
    std::reverse(sol.begin(), sol.end());
    return sol;
}

void Automat::printShortestLengthWords(int word_count, std::ostream& ofs) const
{
    std::vector<int> q_states = {0}, de_unde = {0};
    std::vector<char> last_edge = {'#'};
    std::unordered_map<std::string, bool> accepted_strings;
    int found_words = 0, q_poz = 0;
    while( found_words < word_count && q_poz < (int)q_states.size() ) {
        bool goOn = 1;
        for( const auto& ch_list : states[ q_states[q_poz] ] ) {
            if( !goOn ) 
                break;
            for( const auto& next_node : ch_list.second ) {
                q_states.push_back(next_node);
                last_edge.push_back(ch_list.first);
                de_unde.push_back(q_poz);
                std::string try_string = calc_string(q_states.size() - 1, de_unde, last_edge);
                if( final_states.find(next_node) != final_states.end()
                    && accepted_strings.find(try_string) == accepted_strings.end() ) {
                    accepted_strings[try_string] = 1;
                    ++found_words;
                    if( found_words >= word_count ) {
                        goOn = 0;
                        break;
                    }
                }
            }
        }

        ++q_poz;
    }

    ofs << "Shortest " << word_count << " word(s):\n"; 
    std::vector<std::string> ACStrings;
    for (const auto& pp : accepted_strings)
        ACStrings.push_back(pp.first);
    std::sort(ACStrings.begin(), ACStrings.end(), [](const std::string& a, const std::string& b){
        if (a.size() != b.size())
            return a.size() < b.size();
        return a < b;
    });
    for (const auto& str : ACStrings)
        ofs << str << '\n';
}