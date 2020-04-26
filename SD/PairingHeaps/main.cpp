#include <iostream>

#include "PairingHeap.hpp"

using namespace std;

void MyTest()
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
}

int main()
{
    // MyTest();
    
    /// Interactive Mode!
    PairingHeap heap;
    cout << "Number of operations: ";
    int ops;
    cin >> ops;
    while (ops-- > 0) {
        int type, val;
        cin >> type;
        if (type == 1 || type == 2) {
            cin >> val;
            if (type == 1) {
                heap.Insert(val);
                cout << "Inserted value " << val << '\n';
            }
            else {
                heap.DeleteVal(val);
                cout << "Deleted value " << val << '\n';
            }
        }
        else if (type == 3) {
            cout << "Minimum value: " << heap.GetMin() << '\n';
        }
        else if (type == 4) {
            heap.DeleteMin();
            cout << "Deleted minimum value\n";
        }
    }
    return 0;
}