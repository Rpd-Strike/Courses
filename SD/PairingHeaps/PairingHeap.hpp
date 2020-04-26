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

    /// Creates an empty Pairing Heap
    PairingHeap();

    /// Creates a heap with one element with the given value
    PairingHeap(T val);

    /// Create a heap with given vector of elements
    PairingHeap(std::vector<T> v);

    /// Balance the root and lazy trees
    /// Actually deletes the deleted values that are on the root node
    void Balance();

    /// Inserts a node with the given value
    void Insert(T val);

    /// Deletes the minimum value of the heap
    /// Returns true if a value was deleted
    bool DeleteMin();

    /// Deletes given value from Heap, assumes it is already inside
    void DeleteVal(T val);

    /// Returns the current size of the heap
    int GetSize() const;

    /// Returns the smallest value in the heap
    T GetMin() const;

    /// Returns a PairingHeap built in linear time from the given vector
    static PairingHeap Build(std::vector<T> v);

    /// Joins Heaps A and B and makes A the merged heap
    /// Also returns a reference to A, the merged heap
    static PairingHeap& Merge(PairingHeap& A, PairingHeap& B);
};

#endif // PairingHeap_HPP_