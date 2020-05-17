#ifndef PairingHeap_HPP_
#define PairingHeap_HPP_

#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>
#include <assert.h>

using T = int;

class PairingHeap
{
private:
    /// Heap Node's Structure
    struct Node
    {
        T key;
        std::vector<Node*> sons;

        Node() = delete;
        
        Node(T val) : key(val) {}
        
        ~Node() 
        {
            for (auto ptr : sons)
                delete ptr;
        }
    
        void AddChild(Node* son)
        {
            sons.push_back(son);
        }
    };

    static Node* Join(Node* A, Node* B);

    void PopMin(Node* &tree);

    /// Actual data
    Node* root;
    Node* lazy;
    int   size;

public:
    ~PairingHeap();

    /// Creates an empty Pairing Heap O(1)
    PairingHeap();

    /// Creates a heap with one element with the given value | O(1)
    PairingHeap(T val);

    /// Create a heap with given vector of elements | O(len)
    PairingHeap(std::vector<T> v);

    /// Balance the root and lazy trees
    /// Actually deletes the deleted values that are on the root node | O(logN) amortized
    void Balance();

    /// Inserts a node with the given value O(1)
    void Insert(T val);

    /// Deletes the minimum value of the heap
    /// Returns true if a value was deleted | O(logN) amortized
    bool DeleteMin();

    /// Deletes given value from Heap, assumes it is already inside | O(1)
    void DeleteVal(T val);

    /// Returns the current size of the heap | O(1)
    int GetSize() const;

    /// Returns the smallest value in the heap | O(1)
    T GetMin() const;

    /// Returns a PairingHeap built in linear time from the given vector | O(len)
    static PairingHeap Build(std::vector<T> v);

    /// Joins Heaps A and B and makes A the merged heap
    /// Also returns a reference to A, the merged heap | O(1)
    static PairingHeap& Merge(PairingHeap& A, PairingHeap& B);
};

#endif // PairingHeap_HPP_