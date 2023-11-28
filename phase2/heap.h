// NodeAndHeap.h

#ifndef NODE_AND_HEAP_H
#define NODE_AND_HEAP_H

#include <iostream>
#include <vector>

class Node {
public:
    std::string broker;
    std::string stock;
    int timestamp;
    int exptime;
    char mode;
    int quantity;
    int price;

    Node(std::string b, std::string s, int ts, int et, char m, int q, int p)
        : broker(b), stock(s), timestamp(ts), exptime(et), mode(m), quantity(q), price(p) {}

    // Assuming a proper copy constructor if needed
};

class Heap {
private:
    std::vector<Node*> heap;

    void heapifyUp(int index);
    void heapifyDown(int index);
    bool compareNodes(Node* a, Node* b);

public:
    Heap();
    ~Heap();

    void insert(Node* node);
    Node* extractMin();
    Node* peek();
    Node* getTop();
    bool isEmpty();
    int size();
};

// Implementations

Heap::Heap() {}

Heap::~Heap() {
    for (Node* node : heap) {
        delete node;
    }
}

void Heap::heapifyUp(int index) {
    int parent = (index - 1) / 2;
    while (index > 0 && compareNodes(heap[index], heap[parent])) {
        std::swap(heap[index], heap[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

void Heap::heapifyDown(int index) {
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int smallest = index;

    if (leftChild < heap.size() && compareNodes(heap[leftChild], heap[smallest])) {
        smallest = leftChild;
    }

    if (rightChild < heap.size() && compareNodes(heap[rightChild], heap[smallest])) {
        smallest = rightChild;
    }

    if (smallest != index) {
        std::swap(heap[index], heap[smallest]);
        heapifyDown(smallest);
    }
}

bool Heap::compareNodes(Node* a, Node* b) {
    // Compare based on minimum price, then minimum timestamp, and finally alphabetical order of broker
    if (a->price != b->price) {
        return a->price < b->price;
    } else if (a->timestamp != b->timestamp) {
        return a->timestamp < b->timestamp;
    } else {
        return a->broker < b->broker;
    }
}

void Heap::insert(Node* node) {
    heap.push_back(node);
    heapifyUp(heap.size() - 1);
}

Node* Heap::extractMin() {
    if (isEmpty()) {
        return nullptr;
    }

    Node* root = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    heapifyDown(0);

    return root;
}

Node* Heap::peek() {
    return isEmpty() ? nullptr : heap[0];
}

Node* Heap::getTop() {
    return isEmpty() ? nullptr : heap[0];
}

bool Heap::isEmpty() {
    return heap.empty();
}

int Heap::size() {
    return heap.size();
}

#endif  // NODE_AND_HEAP_H
