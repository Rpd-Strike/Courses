#include <bits/stdc++.h>

using namespace std;

ifstream in("input.in");
ofstream out("output.out");

const int NMAX = 1000;
const int ALFA = 260;

int nr_stari, nr_muchii, stare_initiala;
int nr_finale, intrebari;
unordered_map<int, bool> st_finala;
unordered_map<char, vector<int>> G[NMAX+2];
unordered_map<string, bool> accepted_strings;
vector<int> stari;   /// in ce nod sunt = stari[i]
vector<int> de_unde; /// starea de dinainte
vector<char> last_edge;  ///  muchia folosita sa ajung aici

set<int> undeterministic(const string& str)
{
    set<int> nodes;
    nodes.insert(stare_initiala);
    for( const auto& edge : str ) {
        set<int> after;
        for( auto node : nodes ) {
            for( auto x : G[node][edge] ) {
                after.insert(x);
            }
        }
        nodes = after;
    }
    return nodes;
}

string calc_string(int poz)
{
    string sol;
    while( poz > 0 ) {
        sol.push_back(last_edge[poz]);
        poz = de_unde[poz];
    }
    reverse(sol.begin(), sol.end());
    return sol;
}

void citeste_automat()
{
    in >> nr_stari >> nr_muchii;
    for( int i = 1;  i <= nr_muchii;  ++i ) {
        int from, to;
        char ch;
        in >> from >> to >> ch;
        G[from][ch].push_back(to);
    }
    in >> stare_initiala;
    in >> nr_finale;
    for( int i = 1;  i <= nr_finale;  ++i ) {
        int x;
        in >> x;
        st_finala[x] = 1;
    }
}

void raspunde_intrebari()
{
    in >> intrebari;
    for( int q = 0;  q < intrebari;  ++q ) {
        string str;
        in >> str;
        set<int> nodes = undeterministic(str);
        bool ok = 0;
        for( auto x : nodes ) {
            if( st_finala[x] ) {
                ok = 1;
                break;
            }
        }
        if( ok ) {
            out << str << "    || Accepted\n";
        }
        else {
            out << str << "    || Denied\n";
        }
    }
}

void calc_smallest_100()
{
    stari.push_back(stare_initiala);
    last_edge.push_back('#');
    de_unde.push_back(0);
    int found_words = 0, poz = 0;
    const int NR_FIND = 100;
    while( found_words < NR_FIND && poz < (int)stari.size() ) {
        bool goOn = 1;
        for( auto nextEdge : G[ stari[poz] ] ) {
            if( !goOn ) break;
            for( auto state : nextEdge.second ) {
                stari.push_back(state);
                last_edge.push_back(nextEdge.first);
                de_unde.push_back(poz);
                string try_string = calc_string(stari.size() - 1);
                if( st_finala[state] && accepted_strings.find(try_string) == accepted_strings.end() ) {
                    accepted_strings[try_string] = 1;
                    ++found_words;
                    if( found_words >= NR_FIND ) {
                        goOn = 0;
                        break;
                    }
                }
            }
        }

        ++poz;
    }
}

void afis_smallest_100()
{
    out << "\n\nSmallest 100 words accepted by automaton:\n";

    vector<string>  ACStrings;
    for( auto x : accepted_strings )
        ACStrings.push_back(x.first);
    sort( ACStrings.begin(), ACStrings.end(), [](const string& a, const string& b){
        if( a.size() != b.size() )
            return a.size() < b.size();
        return a < b;
    } );
    for( auto s : ACStrings ) {
        out << s << '\n';
        /// cout << s << '\n';
    }
}

int main()
{
    citeste_automat();
    raspunde_intrebari();
    calc_smallest_100();
    afis_smallest_100();

    return 0;
}
