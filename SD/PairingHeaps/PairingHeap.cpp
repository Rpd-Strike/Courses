#include "PairingHeap.hpp"

PairingHeap::Node* PairingHeap::Join(PairingHeap::Node* A, PairingHeap::Node* B)
{
    if (A == nullptr)
        return B;
    if (B == nullptr)
        return A;

    if (A->key < B->key) {
        A->AddChild(B);
        return A;
    }
    B->AddChild(A);
    return B;
}

void PairingHeap::PopMin(PairingHeap::Node* &tree)
{
    Node* node = nullptr;
    for (size_t i = 0; i < tree->sons.size(); i += 2) {
        // std::cerr << "rem: " << root->sons.size() - i << '\n';
        if (i + 1 < tree->sons.size())
            node = Join(node, Join(tree->sons[i], tree->sons[i + 1]));
        else
            node = Join(node, tree->sons[i]);
    }
    tree->sons.clear();
    delete tree;
    
    tree = node;
}

PairingHeap::~PairingHeap()
{
    delete root;
    delete lazy;
}

/// Creates an empty Pairing Heap
PairingHeap::PairingHeap() : root(nullptr), lazy(nullptr), size(0)
{}

/// Creates a heap with one element with the given value
PairingHeap::PairingHeap(T val) : root(new Node(val)), lazy(nullptr), size(1)
{}

PairingHeap::PairingHeap(std::vector<T> v)
{
    for (auto val : v)
        Insert(val);
}

/// Returns a PairingHeap built in linear time from the given vector
PairingHeap PairingHeap::Build(std::vector<T> v)
{
    PairingHeap ph;
    for (auto val : v)
        ph.Insert(val);
    return ph;
}

/// Returns and  Makes A the merged Heap, makes B empty Heap
PairingHeap& PairingHeap::Merge(PairingHeap& A, PairingHeap& B)
{
    A.size += B.size;
    A.root = Join(A.root, B.root);
    A.lazy = Join(A.lazy, B.lazy);
    B = PairingHeap();
    return A;
}

/// Inserts a node with the given value
void PairingHeap::Insert(T val)
{
    root = Join(root, new Node(val));
    ++size;
}

void PairingHeap::Balance()
{
    while (root != nullptr && lazy != nullptr &&
           root->key == lazy->key) 
    {
        PopMin(root);
        PopMin(lazy);
    }
}

/// Deletes the minimum value of the heap
/// Returns true if a value was deleted
bool PairingHeap::DeleteMin()
{
    if (GetSize() == 0)
        return 0;
    
    Balance();
    PopMin(root);
    
    --size;
    return 1;
}

void PairingHeap::DeleteVal(T val)
{
    lazy = Join(lazy, new Node(val));
    --size;
}

/// Returns the current size of the heap
int PairingHeap::GetSize() const
{
    return size;
}

T PairingHeap::GetMin() const
{
    if (GetSize() == 0) 
        throw std::runtime_error("Pairing heap is empty, no minimum to get!");
    return root->key;
}