#include "heapSort.hpp"

namespace Sortari
{
    void pushHeap(int N, int val, std::vector<int>& v)
    {
        v[N + 1] = val;
        int pos = N + 1;
        while (pos > 1 && v[pos / 2] < v[pos]) {
            std::swap(v[pos / 2], v[pos]);
            pos /= 2;
        }
    }

    int biggestChild(int heapSize, int poz, std::vector<int>& v)
    {
        assert(poz * 2 <= heapSize);
        int bigChild = poz * 2;
        if (poz * 2 + 1 <= heapSize) if (v[poz * 2 + 1] > v[bigChild])
            bigChild = poz * 2 + 1;
        return bigChild; 
    }

    int popHeap(int N, std::vector<int>& v)
    {
        std::swap(v[1], v[N]);
        int pos = 1, bigChild = 0;
        while (pos * 2 < N && v[(bigChild = biggestChild(N - 1, pos, v))] > v[pos]) {
            std::swap(v[pos], v[bigChild]);
            pos = bigChild;
        }
        return v[N];
    }

    std::string HeapSort(std::vector<int>& v)
    {
        using std::swap;

        v.push_back(0);
        swap(v[0], v.back());
        /// elemente de la 1 la N
        for (size_t i = 1; i < v.size(); ++i) {
            pushHeap(i - 1, v[i], v);
        }
        for (size_t i = 1; i < v.size(); ++i) {
            int val = popHeap(v.size() - i, v);
            v[v.size() - i] = val;
        }
        for (size_t i = 0; i + 1 < v.size(); ++i)
            v[i] = v[i + 1];
        v.pop_back();

        return "";
    }
}