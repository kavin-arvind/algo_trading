#include<iostream>
#include <fstream>
#include <string>
#include <vector>

// AVLMap.h
#ifndef AVLMAP_H
#define AVLMAP_H

template <typename T>
const T& customMax(const T& a, const T& b) {
    return (a > b) ? a : b;
}

class AVLMap {
private:
    struct Node {
        std::string key;
        std::vector<int> value;
        Node* left;
        Node* right;
        int height;

        Node(const std::string& k, const std::vector<int>& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node);
    int balanceFactor(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* leftRightRotate(Node* x);
    Node* rightLeftRotate(Node* y);
    Node* insert(Node* node, const std::string& key, const std::vector<int>& value);
    Node* minValueNode(Node* node);
    Node* deleteNode(Node* root, const std::string& key);
    Node* find(Node* node, const std::string& key);

public:
    AVLMap();
    ~AVLMap();

    void insert(const std::string& key, const std::vector<int>& value);
    void remove(const std::string& key);
    bool containsKey(const std::string& key);
    std::vector<int> getValue(const std::string& key);
};

// Constructor
AVLMap::AVLMap() : root(nullptr) {}

// Destructor
AVLMap::~AVLMap() {
    // TODO: Implement code to delete all nodes in the AVLMap
}

// Helper function to get the height of a node
int AVLMap::height(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

void AVLMap::insert(const std::string& key, const std::vector<int>& value) {
    root = insert(root , key, value);
}

// Helper function to calculate the balance factor of a node
int AVLMap::balanceFactor(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

// Helper function for right rotation
AVLMap::Node* AVLMap::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + std::max(height(y->left), height(y->right));
    x->height = 1 + std::max(height(x->left), height(x->right));

    return x;
}

// Helper function for left rotation
AVLMap::Node* AVLMap::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));

    return y;
}

// Helper function for left-right rotation
AVLMap::Node* AVLMap::leftRightRotate(Node* x) {
    x->left = leftRotate(x->left);
    return rightRotate(x);
}

// Helper function for right-left rotation
AVLMap::Node* AVLMap::rightLeftRotate(Node* y) {
    y->right = rightRotate(y->right);
    return leftRotate(y);
}

// Helper function to insert a key-value pair into the AVLMap
AVLMap::Node* AVLMap::insert(Node* node, const std::string& key, const std::vector<int>& value) {
    // Base case
    if (node == nullptr) {
        return new Node(key, value);
    }

    // Insert the node
    if (key < node->key) {
        node->left = insert(node->left, key, value);
    } else if (key > node->key) {
        node->right = insert(node->right, key, value);
    } else {
        // Key already exists, update the value
        node->value = value;
        return node;
    }

    // Update height of current node
    node->height = 1 + std::max(height(node->left), height(node->right));

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
AVLMap::Node* AVLMap::minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// Helper function to delete a node with a given key from the AVLMap
AVLMap::Node* AVLMap::deleteNode(Node* root, const std::string& key) {
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
            root->value = temp->value;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node, return
    if (root == nullptr) {
        return root;
    }

    // Update height of the current node
    root->height = 1 + std::max(height(root->left), height(root->right));

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
AVLMap::Node* AVLMap::find(Node* node, const std::string& key) {
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

// Function to remove a key and its associated value from the AVLMap
void AVLMap::remove(const std::string& key) {
    root = deleteNode(root, key);
}

// Function to check if a key is present in the AVLMap
bool AVLMap::containsKey(const std::string& key) {
    return find(root, key) != nullptr;
}

// Function to get the value associated with a key in the AVLMap
std::vector<int> AVLMap::getValue(const std::string& key) {
    Node* node = find(root, key);
    if (node != nullptr) {
        return node->value;
    } else {
        // Return an empty vector if the key is not found
        return std::vector<int>();
    }
}

#endif  // AVLMAP_H


// AVLMap2.h
#ifndef AVLMAP2_H
#define AVLMAP2_H

// NodeAndHeap.h

#ifndef NODE_AND_HEAP_H
#define NODE_AND_HEAP_H


struct Node {
//public:
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
    std::cerr<<"ind"<<index<<std::endl;
    while (index > 0 && compareNodes(heap[index], heap[parent])) {
        std::cerr<<"h2"<<std::endl;
        //std::swap(heap[index], heap[parent]);
        std::cerr<<"h3"<<std::endl;
        index = parent;
        std::cerr<<"h4"<<std::endl;
        parent = (index - 1) / 2;
        std::cerr<<"h5"<<std::endl;
    }
    std::cerr<<"h0"<<std::endl;
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
    std::cerr<<"cn"<<std::endl;
    std::cerr<<"a price:"<<a<<std::endl;
    // if(a->price){std::cerr<<"q1"<<std::endl;}
    // else if(b->price){std::cerr<<"q2"<<std::endl;}
    std::cerr<<"cn2"<<std::endl;
    if (a->price != b->price) {
        return a->price < b->price;
    } else if (a->timestamp != b->timestamp) {
        return a->timestamp < b->timestamp;
    } else {
        return a->broker < b->broker;
    }
}

void Heap::insert(Node* node) {
    std::cerr<<node->price<<std::endl;
    heap.push_back(node);
    std::cerr<<node->price<<std::endl;
    std::cerr<<"hmm"<<std::endl;
    std::cerr<<heap.size()<<std::endl;
    std::cerr<<heap[0]<<std::endl;
    std::cerr<<'0'<<std::endl;
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


// struct Node {
//     std::string broker;
//     std::string stock;
//     int timestamp;
//     int exptime;
//     char mode;
//     int quantity;
//     int price;

//     Node(std::string b = "", std::string s = "", int ts = 0, int et = 0, char m = 'n', int q = 0, int p = 0)
//         : broker(b), stock(s), timestamp(ts), exptime(et), mode(m), quantity(q), price(p) {}
// };

bool valid_for_substr(std::string str, int startPos, int length){
    if (startPos < str.size() && 0<=length && length <= str.size()-startPos) {
        return true;
    } else {
        return false;
    }
}

Node* line_process_to_node(std::string line, bool &valid){
    if(line == "TL"){valid = false; return nullptr;}
    Node* inp = new Node() ;
    valid = true;

    int linear_combo_start = 0;

    //coming from start

    int index = 0;
    int index_prev = 0;
    // timestamp:
    while(line[index] != ' '){
        index++;
    }
    if(valid_for_substr(line,index_prev, index-index_prev)){
        try{inp->timestamp = std::stoi(line.substr(index_prev,index-index_prev));}
        catch (const std::invalid_argument& e){
            inp->timestamp = 0; valid = false; return inp;
        }
    }
    else{valid = false; return inp;}
    index_prev = ++index;

    // broker:
    while(line[index] != ' '){
        index++;
    }

    if(valid_for_substr(line,index_prev, index-index_prev)){
        try{inp->broker = line.substr(index_prev,index-index_prev);}
        catch (const std::invalid_argument& e){
            inp->broker = ""; valid = false; return inp;
        }
    }
    else{valid = false; return inp;}
    index_prev = ++index;

    // b/s:
    while(line[index] != ' '){
        index++;
    }
    if(valid_for_substr(line,index_prev, index-index_prev)){
        if(line.substr(index_prev,index-index_prev) == "SELL"){inp->mode = 's';}
        else if(line.substr(index_prev,index-index_prev) == "BUY"){inp->mode = 'b';}
        else{inp->mode = 'n'; valid = false; return inp;}
    }
    else{valid = false; return inp;}
    index_prev = ++index;

    linear_combo_start = index_prev ; 
    // coming from back for others

    index = line.size()-1;
    index_prev = line.size();
    
    // expairy:
    inp->exptime = inp->timestamp;
    while(line[index] != ' '){
        index--;
    }
    if(valid_for_substr(line,index+1, index_prev - index - 1)){
        try{
            int temp = std::stoi(line.substr(index+1,index_prev - index - 1));
            if(temp == -1){inp->exptime = -1;}
            else{inp->exptime += temp;}
        }
        catch (const std::invalid_argument& e){
            inp->exptime += 0; valid = false; return inp;
        }
    }
    else{valid = false;}
    index_prev = index--;

    // quantity:
    while(line[index] != '#'){
        index--;
    }
    if(valid_for_substr(line,index+1, index_prev - index - 1)){
        try{inp->quantity = std::stoi(line.substr(index+1,index_prev - index - 1));}
        catch (const std::invalid_argument& e){
            inp->quantity = 0; valid = false; return inp;
        }
    }
    else{valid = false; return inp;}
    index--; index_prev = index--;

    if(line[index_prev]!=' '){valid = false; return inp;}

    // price:
    while(line[index] != '$'){
        index--;
    }
    if(valid_for_substr(line,index+1, index_prev - index - 1)){

        try{inp->price = std::stoi(line.substr(index+1,index_prev - index - 1));}
        catch (const std::invalid_argument& e){
            inp->price = 0; valid = false; return inp;
        }
    }
    else{valid = false; return inp;}
    index--; index_prev = index--; 

    if(line[index_prev]!=' '){valid = false; return inp;}
   

    if(valid_for_substr(line,linear_combo_start, index_prev - linear_combo_start)){
        try{inp->stock = line.substr(linear_combo_start,index_prev - linear_combo_start);}
        catch (const std::invalid_argument& e){
            inp->stock = ""; valid = false; return inp;
        }
    }
    else{
        valid = false; return inp;
    }

    if(inp->mode == 'b'){
        inp->price = (-1)*inp->price;
    }

    return inp;

}