// AVLMap2.h
#ifndef AVLMAP2_H
#define AVLMAP2_H

#include <iostream>
#include <vector>
#include "Heap.h" // Assuming you have a Heap class

template <typename T>
const T& customMax(const T& a, const T& b) {
    return (a > b) ? a : b;
}

class AVLMap2 {
private:
    struct Node {
        std::string key;
        Heap value; // Assuming Heap is a class you have defined
        Node* left;
        Node* right;
        int height;

        Node(const std::string& k, const Heap& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node);
    int balanceFactor(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* leftRightRotate(Node* x);
    Node* rightLeftRotate(Node* y);
    Node* insert(Node* node, const std::string& key, const Heap& value);
    Node* minValueNode(Node* node);
    Node* deleteNode(Node* root, const std::string& key);
    Node* find(Node* node, const std::string& key);

public:
    AVLMap2();
    ~AVLMap2();

    void insert(const std::string& key, const Heap& value);
    void remove(const std::string& key);
    bool containsKey(const std::string& key);
    Heap getValue(const std::string& key);
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
AVLMap2::Node* AVLMap2::insert(Node* node, const std::string& key, const Heap& value) {
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
void AVLMap2::insert(const std::string& key, const Heap& value) {
    root = insert(root, key, value);
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
Heap AVLMap2::getValue(const std::string& key) {
    Node* node = find(root, key);
    if (node != nullptr) {
        return node->value;
    } else {
        // Return a default-constructed Heap if the key is not found
        return Heap(); // Assuming Heap has a default constructor
    }
}

#endif  // AVLMAP2_H
