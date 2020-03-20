#include "stlSort.hpp"

namespace Sortari
{
    std::string STLSort(std::vector<int>& v)
    {
        sort(v.begin(), v.end());
        return "";
    }
}