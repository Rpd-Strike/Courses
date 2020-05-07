#include "radixSort.hpp"

#include "Felix/Colors.hpp"

namespace Sortari
{
    std::string RadixSort(std::vector<int>& v)
    {
        using Felix::Colors::File::red;
        using Felix::Colors::File::reset;

        const int BASE = 10;
        const int valMax = *std::max_element(v.begin(), v.end());
        const int valMin = *std::min_element(v.begin(), v.end());
        if (valMin < 0) {
            return red + "Negative values don't work with Radix Sort" + reset;
        }
        std::queue<int> q[2][BASE];

        for( auto & x : v )
            q[0][x % BASE].push(x);
        int qind = 0, put = BASE;

        while(valMax >= put) {
            // cout << "hei\n";
            qind ^= 1;

            for( int i = 0;  i < BASE;  ++i ) {
                while( !q[1 ^ qind][i].empty() ) {
                    int x = q[1 ^ qind][i].front();
                    //std::cerr << x / put % BASE << ' ';
                    q[1 ^ qind][i].pop();
                    q[qind][x / put % BASE].push(x);
                }
            }
            /// std::cerr << '\n';
            if( valMax / put < BASE )
                break;
            put *= BASE;    
        }
        v.clear();
        for( int i = 0;  i < BASE;  ++i ) 
            while( !q[qind][i].empty() )
                v.push_back(q[qind][i].front()),
                q[qind][i].pop();
        return "";  
    }
}