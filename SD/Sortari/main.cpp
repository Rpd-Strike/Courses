#include <vector>
#include <limits>
#include <iomanip>
#include <fstream>

#include "countSort.hpp"
#include "quickSort.hpp"
#include "mergeSort.hpp"
#include "radixSort.hpp"
#include "bubbleSort.hpp"
#include "heapSort.hpp"
#include "stlSort.hpp"

#include "Felix/Ticker.hpp"
#include "Felix/Random.hpp"
#include "Felix/Colors.hpp"

const int NR_GEN = 1e7;
const int MIC    = 0;
const int MARE   = 1e6 - 1;

std::vector<int> askUserForValues(int& afis)
{
    using namespace Felix::Colors::Console;
    using std::cout;
    using std::cin;

    int nr = NR_GEN, minim = MIC, maxim = MARE;
    std::string str;
    afis = 0;
    
    cout << "\nCate numere: ";
    cin >> nr;
    cout << "Valoare minima: ";
    cin >> minim;
    cout << "valoare maxima: ";
    cin >> maxim;
    cout << "Afisati vectorul generat si listele sortate? [y/n]: ";
    cin >> str;
    if (str[0] == 'y' || str[0] == 'Y')
        afis = 1;

    int minim_v = std::numeric_limits<int>::max();
    int maxim_v = std::numeric_limits<int>::min();
    std::vector<int> v(nr);
    for (int &x : v) {
        x = Felix::Random::generateRandom(minim, maxim);
        minim_v = std::min(minim_v, x);
        maxim_v = std::max(maxim_v, x);
        if (afis)
            cout << x << ' ';
    }
    
    cout << yellow << "\nGenerat " << nr << " numere, valori in: ";
    cout << "[" << minim_v << ", " << maxim_v << "]\n\n" << reset;
    return v;
}

std::vector<int> generateValues(int vmin, int vmax, int count)
{
    std::vector<int> v(count);
    for (int &x : v) {
        x = Felix::Random::generateRandom(vmin, vmax);
    }
    return v;
}

void testSort(std::vector<int> valori, std::function<std::string(std::vector<int>&)> sortFunction, 
              const std::string& sortName, std::ostream& outs, int afis = 0)
{
    using namespace Felix::Colors::File;

    outs << cyan + sortName + reset << "  ... ";

    Felix::Ticker::Ticker myClock;
    auto message = sortFunction(valori);
    double seconds = myClock.secondsSinceLastClock();

    bool ok = 1;
    for (size_t i = 0; i < valori.size(); ++i) {
        if (afis)
            outs << valori[i] << ' ';
        if (i > 0)
            ok &= (valori[i - 1] <= valori[i]);
    }

    outs << std::fixed << std::setprecision(3);
    outs << " ..... " << message;
    outs << std::string((message != "" ? "  " : "")) + ".........  " << (ok ? green + "Corect" : red + "Pe langa") << reset <<  "  ...  ";
    outs << seconds << " s";
    outs << "\n";
}

int main()
{
    Felix::Ticker::testFunction();

    int afis = 0;
    std::ifstream fin("test.txt");
    std::ofstream fout("results.txt");
    fout << "Tested different types of sorts:'\n";
    
    int nrTests;
    fin >> nrTests;
    for (int nr = 0; nr < nrTests; ++nr) {
        int vmin, vmax, count;
        fin >> vmin >> vmax >> count;
        
        auto valori = generateValues(vmin, vmax, count);

        fout << "\nValori: [" << vmin << ", " << vmax << "]  N = " << count << "\n"; 

        testSort(valori, Sortari::STLSort,    "STL    Sort    ", fout, afis);
        testSort(valori, Sortari::RadixSort,  "Radix  Sort    ", fout, afis);
        testSort(valori, Sortari::MergeSort,  "Merge  Sort    ", fout, afis);
        testSort(valori, Sortari::HeapSort,   "Heap   Sort    ", fout, afis);
        testSort(valori, Sortari::CountSort,  "Count  Sort    ", fout, afis);
        testSort(valori, Sortari::QuickSort1, "Quick  Sort - 1", fout, afis);
        testSort(valori, Sortari::QuickSort3, "Quick  Sort - 3", fout, afis);
        testSort(valori, Sortari::QuickSort5, "Quick  Sort - 5", fout, afis);
        testSort(valori, Sortari::BubbleSort, "Bubble Sort    ", fout, afis);

        std::cout << "test " << nr + 1 << " finished'\n";
    }   

    return 0;
}