#include "quickSort.hpp"

#include "Felix/Random.hpp"

namespace Sortari
{
    using Felix::Random::generateRandom;

    static int PickPivotFrom5(const std::vector<int>& v, int from, int to)
    {
        if (to - from < 5)
            return v[from + (to - from) / 2];
        std::vector<int> vals(5);
        for (int& x : vals)
            x = v[generateRandom(from, to)];
        for (int i = 1; i < 5; ++i) if (vals[0] > vals[i])
            std::swap(vals[0], vals[i]);
        for (int i = 1; i < 4; ++i) if (vals[4] < vals[i])
            std::swap(vals[4], vals[i]);
        if (vals[3] > vals[2])
            std::swap(vals[2], vals[3]);
        if (vals[3] > vals[1])
            std::swap(vals[3], vals[1]);
        return std::min(vals[2], vals[3]);
    }

    static int PickPivotFrom3(const std::vector<int>& v, int from, int to)
    {
        if (to - from < 3)
            return v[from];
        int a = v[generateRandom(from, to)];
        int b = v[generateRandom(from, to)];
        int c = v[generateRandom(from, to)];
        if (c > b)
            std::swap(c, b);
        if (c > a)
            std::swap(c, a);
        return std::min(a, b);
    }

    static int PickPivotFrom1(const std::vector<int>& v, int from, int to)
    {
        return v[generateRandom(from, to)];
    }

    static auto PickPivot = PickPivotFrom1;

    static int Partition(std::vector<int>& v, int from, int to)
    {
        int val = PickPivot(v, from, to);
        size_t st = from, dr = to, i = from, pos_val = from, middle = from;
        while (st <= dr) {
            if (v[i] < val)
                std::swap(v[st++], v[i++]);
            else if (v[i] > val)
                std::swap(v[dr--], v[i]);
            else {
                std::swap(v[st++], v[i++]);
                pos_val = st - 1;
            }
            middle = st - 1;
        }

        assert(v[pos_val] == val);
        std::swap(v[middle], v[pos_val]);

        // for ( int j = from;  j <= to;  ++j ) {
        //     std::cout << "_=*"[(v[j] >= val) + (v[j] > val)];
        // }
        // std::cout << '\n';

        return middle;
    }

    static void stepQuickSort(std::vector<int>& v, int from, int to)
    {
        if (to - from < 1)
            return ;
        if (to - from == 1) {
            if (v[from] > v[to])
                std::swap(v[from], v[to]);
            return ;
        }

        assert(0 <= from && from < (int)v.size());

        int poz = Partition(v, from, to);  /// [from, poz | poz + 1, to]
        stepQuickSort(v, from, poz - 1);
        stepQuickSort(v, poz + 1, to);
    }

    std::string QuickSort1(std::vector<int>& v)
    {
        PickPivot = PickPivotFrom1;
        stepQuickSort(v, 0, v.size() - 1);
        return "";
    }

    std::string QuickSort3(std::vector<int>& v)
    {
        PickPivot = PickPivotFrom3;
        stepQuickSort(v, 0, v.size() - 1);
        return "";
    }

    std::string QuickSort5(std::vector<int>& v)
    {
        PickPivot = PickPivotFrom5;
        stepQuickSort(v, 0, v.size() - 1);
        return "";
    }

    #undef _RandomPickCount
    #undef __PP_two
    #undef __PP_one
    #undef PickPivot
}