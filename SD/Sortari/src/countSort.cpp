#include "countSort.hpp"

#include "Felix/Colors.hpp"

namespace Sortari
{
    std::string CountSort(std::vector<int>& v)
    {
        using Felix::Colors::File::red;
        using Felix::Colors::File::reset;

        if( v.empty() ) 
            return "";
        int valmin = *std::min_element(v.begin(), v.end());
        int valmax = *std::max_element(v.begin(), v.end());
        if( valmax - valmin > VALMAX ) {
            return red + "Values from given vector too big for CountSort" + reset;
        }
        std::vector<int> values(VALMAX + 2, 0);
        for( auto & x : v )
            ++values[x - valmin];
        v.clear();
        for ( int i = 0;  i <= VALMAX;  ++i )
            for( int j = 0;  j < values[i];  ++j )
                v.push_back(i + valmin);
        return "";
    }
}