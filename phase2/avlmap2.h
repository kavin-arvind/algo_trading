// AVLMap2.h
#ifndef AVLMAP2_H
#define AVLMAP2_H

// NodeAndHeap.h

#ifndef NODE_AND_HEAP_H
#define NODE_AND_HEAP_H


class Node {
public:
    std::string broker;
    std::string stock;
    int timestamp;
    int exptime;
    char mode;
    int quantity;
    int price;

    Node(std::string b = "", std::string s = "", int ts = 0, int et = 0, char m = 'n', int q = 0, int p = 0)
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


// template <typename T>
// const T& customMax(const T& a, const T& b) {
//     return (a > b) ? a : b;
// }

class AVLMap2 {
private:
    struct Node {
        std::string key;
        Heap* valuePointer; // Change Heap to Heap*
        Node* left;
        Node* right;
        int height;

        Node(const std::string& k, Heap* v)
            : key(k), valuePointer(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node);
    int balanceFactor(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* leftRightRotate(Node* x);
    Node* rightLeftRotate(Node* y);
    Node* insert(Node* node, const std::string& key, Heap* valuePointer);
    Node* minValueNode(Node* node);
    Node* deleteNode(Node* root, const std::string& key);
    Node* find(Node* node, const std::string& key);

public:
    AVLMap2();
    ~AVLMap2();

    void insert(const std::string& key, Heap* valuePointer);
    void remove(const std::string& key);
    bool containsKey(const std::string& key);
    Heap* getValue(const std::string& key);
};

// Constructor
AVLMap2::AVLMap2() : root(nullptr) {}

// Destructor
AVLMap2::~AVLMap2() {
    // TODO: Implement code to delete all nodes in the AVLMap
}

// Helper function to get the height of a node
int AVLMap2::height(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

// Helper function to calculate the balance factor of a node
int AVLMap2::balanceFactor(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

// Helper function for right rotation
AVLMap2::Node* AVLMap2::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + customMax(height(y->left), height(y->right));
    x->height = 1 + customMax(height(x->left), height(x->right));

    return x;
}

// Helper function for left rotation
AVLMap2::Node* AVLMap2::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + customMax(height(x->left), height(x->right));
    y->height = 1 + customMax(height(y->left), height(y->right));

    return y;
}

// Helper function for left-right rotation
AVLMap2::Node* AVLMap2::leftRightRotate(Node* x) {
    x->left = leftRotate(x->left);
    return rightRotate(x);
}

// Helper function for right-left rotation
AVLMap2::Node* AVLMap2::rightLeftRotate(Node* y) {
    y->right = rightRotate(y->right);
    return leftRotate(y);
}

// Helper function to insert a key-value pair into the AVLMap
AVLMap2::Node* AVLMap2::insert(Node* node, const std::string& key, Heap* valuePointer) {
    // Base case
    if (node == nullptr) {
        return new Node(key, valuePointer);
    }

    // Insert the node
    if (key < node->key) {
        node->left = insert(node->left, key, valuePointer);
    } else if (key > node->key) {
        node->right = insert(node->right, key, valuePointer);
    } else {
        // Key already exists, update the valuePointer
        node->valuePointer = valuePointer;
        return node;
    }

    // Update height of current node
    node->height = 1 + customMax(height(node->left), height(node->right));

    // Get the balance factor
    int balance = balanceFactor(node);

    // Perform rotations if needed
    if (balance > 1) {
        if (key < node->left->key) {
            return rightRotate(node);
        } else {
            return leftRightRotate(node);
        }
    }
    if (balance < -1) {
        if (key > node->right->key) {
            return leftRotate(node);
        } else {
            return rightLeftRotate(node);
        }
    }

    return node;
}

// Helper function to find the node with the minimum value in a subtree
AVLMap2::Node* AVLMap2::minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// Helper function to delete a node with a given key from the AVLMap
AVLMap2::Node* AVLMap2::deleteNode(Node* root, const std::string& key) {
    // Base case
    if (root == nullptr) {
        return root;
    }

    // Perform standard BST delete
    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        // Node with only one child or no child
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                // One child case
                *root = *temp;
            }

            delete temp;
        } else {
            // Node with two children, get the inorder successor
            Node* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->key = temp->key;
            root->valuePointer = temp->valuePointer;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node, return
    if (root == nullptr) {
        return root;
    }

    // Update height of the current node
    root->height = 1 + customMax(height(root->left), height(root->right));

    // Get the balance factor
    int balance = balanceFactor(root);

    // Perform rotations if needed
    if (balance > 1) {
        if (balanceFactor(root->left) >= 0) {
            return rightRotate(root);
        } else {
            return leftRightRotate(root);
        }
    }
    if (balance < -1) {
        if (balanceFactor(root->right) <= 0) {
            return leftRotate(root);
        } else {
            return rightLeftRotate(root);
        }
    }

    return root;
}

// Helper function to find a node with a given key in the AVLMap
AVLMap2::Node* AVLMap2::find(Node* node, const std::string& key) {
    // Base cases
    if (node == nullptr || node->key == key) {
        return node;
    }

    // Key is smaller than the root's key
    if (key < node->key) {
        return find(node->left, key);
    }

    // Key is larger than the root's key
    return find(node->right, key);
}

// Function to insert a key-value pair into the AVLMap
void AVLMap2::insert(const std::string& key, Heap* valuePointer) {
    root = insert(root, key, valuePointer);
}

// Function to remove a key and its associated value from the AVLMap
void AVLMap2::remove(const std::string& key) {
    root = deleteNode(root, key);
}

// Function to check if a key is present in the AVLMap
bool AVLMap2::containsKey(const std::string& key) {
    return find(root, key) != nullptr;
}

// Function to get the value associated with a key in the AVLMap
Heap* AVLMap2::getValue(const std::string& key) {
    Node* node = find(root, key);
    if (node != nullptr) {
        return node->valuePointer;
    } else {
        // Return nullptr if the key is not found
        return nullptr;
    }
}

#endif  // AVLMAP2_H
