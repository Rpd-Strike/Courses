#include "mergeSort.hpp" 

namespace Sortari
{
    std::string MergeSort(std::vector<int>& v)
    {
        std::vector<int> arr(v.size());
        std::function<void(int,int)> stepSort = [&](int st, int dr){
            if( st >= dr )
                return ;
            int mid = (st + dr) / 2;
            stepSort(st, mid);
            stepSort(mid + 1, dr);
            int j = mid + 1, ar_ind = 0;
            for( int i = st;  i <= mid;  ++i ) {
                while( j <= dr && v[j] < v[i] )
                    arr[ar_ind++] = v[j++];
                arr[ar_ind++] = v[i];
            }
            while( j <= dr )
                arr[ar_ind++] = v[j++];
            for( int i = 0;  i < ar_ind;  ++i )
                v[i + st] = arr[i];
        };

        stepSort(0, v.size() - 1);

        return "";
    }
}