#ifndef __AUTOMAT_HPP__
#define __AUTOMAT_HPP__

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

class Automat 
{
private:
    typedef std::unordered_map<char, std::vector<int>> edgeList;
    
    std::vector<edgeList> states;
    int nr_states, start_state;
    std::unordered_map<int, bool> final_states;
public:
    Automat();
    Automat(int nr_nodes);
    friend std::istream& operator >> (std::istream& ifs, Automat& Atm);
    bool isWordAccepted(const std::string& str) const ;
    void printShortestLengthWords(int word_count, std::ostream& ofs) const ;
};

#endif