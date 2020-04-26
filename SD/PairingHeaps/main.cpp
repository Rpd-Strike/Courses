#include <iostream>

#include "PairingHeap.hpp"

using namespace std;

int main()
{
    auto ptree = PairingHeap::Build({5, 8, 1});
    auto doi = PairingHeap::Build({6, 9});
    

    auto& tree = PairingHeap::Merge(ptree, doi);




    std::cout << "gata\n";
    while (tree.GetSize() > 0) {
        cout << tree.GetSize() << ' ';
        cout << tree.GetMin() << '\n';
        if (tree.GetSize() > 0)
            tree.DeleteMin();
    }

    tree.Insert(4);
    tree.Insert(7);
    tree.Insert(3);
    tree.Insert(2);

    tree.DeleteVal(2);
    tree.DeleteMin();
    tree.DeleteVal(7);

    std::cout << "gata\n";
    while (tree.GetSize() > 0) {
        cout << tree.GetSize() << ' ';
        cout << tree.GetMin() << '\n';
        if (tree.GetSize() > 0)
            tree.DeleteMin();
    }



    //  delete doi;

    return 0;
}