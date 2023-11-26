#ifndef MAP_AVL_H
#define MAP_AVL_H

#include <vector>
#include <string>
#include <iostream>

class AVLMap {
private:
    struct Node {
        std::string key;
        int value;
        int height;
        Node* left;
        Node* right;

        Node(const std::string& k, int v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    int getHeight(Node* node);
    int getBalanceFactor(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* insert(Node* node, const std::string& key, int value);
    Node* minValueNode(Node* node);
    Node* remove(Node* root, const std::string& key);
    void clearRecursive(Node* node);
    int getSizeRecursive(Node* node) const;
    Node* update(Node* node, const std::string& key, int value);

public:
    AVLMap();
    ~AVLMap();

    void insert(const std::string& key, int value);
    int getValue(const std::string& key) const;
    bool containsKey(const std::string& key) const;
    void remove(const std::string& key);
    int getSize() const;
    bool isEmpty() const;
    void clear();
    void update(const std::string& key, int value);
};

AVLMap::AVLMap() : root(nullptr) {}

AVLMap::~AVLMap() {
    clearRecursive(root);
}

void AVLMap::clearRecursive(Node* node) {
    if (node != nullptr) {
        clearRecursive(node->left);
        clearRecursive(node->right);
        delete node;
    }
}

int AVLMap::getHeight(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int AVLMap::getBalanceFactor(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

AVLMap::Node* AVLMap::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLMap::Node* AVLMap::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

AVLMap::Node* AVLMap::insert(Node* node, const std::string& key, int value) {
    if (node == nullptr) {
        return new Node(key, value);
    }

    if (key < node->key) {
        node->left = insert(node->left, key, value);
    } else if (key > node->key) {
        node->right = insert(node->right, key, value);
    } else {
        return node;
    }

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1 && key < node->left->key) {
        return rightRotate(node);
    }

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key > node->right->key) {
        return leftRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLMap::insert(const std::string& key, int value) {
    root = insert(root, key, value);
}

AVLMap::Node* AVLMap::minValueNode(Node* node) {
    Node* current = node;

    while (current->left != nullptr) {
        current = current->left;
    }

    return current;
}

AVLMap::Node* AVLMap::remove(Node* root, const std::string& key) {
    if (root == nullptr) {
        return root;
    }

    if (key < root->key) {
        root->left = remove(root->left, key);
    } else if (key > root->key) {
        root->right = remove(root->right, key);
    } else {
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }

            delete temp;
        } else {
            Node* temp = minValueNode(root->right);

            root->key = temp->key;
            root->value = temp->value;

            root->right = remove(root->right, temp->key);
        }
    }

    if (root == nullptr) {
        return root;
    }

    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));

    int balance = getBalanceFactor(root);

    if (balance > 1 && getBalanceFactor(root->left) >= 0) {
        return rightRotate(root);
    }

    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalanceFactor(root->right) <= 0) {
        return leftRotate(root);
    }

    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void AVLMap::remove(const std::string& key) {
    root = remove(root, key);
}

int AVLMap::getValue(const std::string& key) const {
    Node* current = root;

    while (current != nullptr) {
        if (key == current->key) {
            return current->value;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return 0;
}

bool AVLMap::containsKey(const std::string& key) const {
    Node* current = root;

    while (current != nullptr) {
        if (key == current->key) {
            return true;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return false;
}

int AVLMap::getSize() const {
    return getSizeRecursive(root);
}

int AVLMap::getSizeRecursive(Node* node) const {
    if (node == nullptr) {
        return 0;
    }
    return 1 + getSizeRecursive(node->left) + getSizeRecursive(node->right);
}

bool AVLMap::isEmpty() const {
    return root == nullptr;
}

void AVLMap::clear() {
    clearRecursive(root);
    root = nullptr;
}

AVLMap::Node* AVLMap::update(Node* node, const std::string& key, int value) {
    // If the node is null, the key is not present, so we insert the key-value pair
    if (node == nullptr) {
        return insert(node, key, value);
    }

    // If the key is less than the current node's key, go left
    if (key < node->key) {
        node->left = update(node->left, key, value);
    }
    // If the key is greater than the current node's key, go right
    else if (key > node->key) {
        node->right = update(node->right, key, value);
    }
    // If the key is equal to the current node's key, update the value
    else {
        node->value = value;
        return node;
    }

    // Update height of the current node
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // Get the balance factor to check if rotation is needed
    int balance = getBalanceFactor(node);

    // Left Left Case
    if (balance > 1 && key < node->left->key) {
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && key > node->right->key) {
        return leftRotate(node);
    }

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // No rotation needed
    return node;
}

void AVLMap::update(const std::string& key, int value) {
    root = update(root, key, value);
}

#endif // MAP_AVL_H

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct Node {
    std::vector<int> data;
    int price;      // New addition: an integer price
    std::string inpline;  // New addition: a string named inpline
    char mode;      // New addition: a char named mode
    Node* next;

    // Constructor to initialize a node with data, a default price of 0, an empty inpline, and 'N' as the default mode
    Node(const std::vector<int>& data, int price = 0, const std::string& inpline = "", char mode = 'N')
        : data(data), price(price), inpline(inpline), mode(mode), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    Node* tail;  // Add a tail pointer to keep track of the end of the list
    int size;    // Add a size variable to keep track of the number of nodes in the list

public:
    // Constructor to initialize an empty linked list
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Destructor to free memory when the linked list is destroyed
    ~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Function to add a vector with a price, inpline, and mode to the end of the linked list
    void addVector(const std::vector<int>& data, int price = 0, const std::string& inpline = "", char mode = 'N') {
        Node* newNode = new Node(data, price, inpline, mode);
        if (!head) {
            // If the list is empty, both head and tail point to the new node
            head = tail = newNode;
        } else {
            // Otherwise, add the new node at the end and update the tail
            tail->next = newNode;
            tail = newNode;
        }

        size++;  // Increment the size when a new node is added
    }

    // Function to delete a vector from the linked list based on its content
    void deleteVector(const std::vector<int>& data) {
        Node* current = head;
        Node* prev = nullptr;

        while (current) {
            if (current->data == data) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    head = current->next;
                }

                delete current;
                size--;  // Decrement the size when a node is deleted
                return;  // Assuming each vector is unique, exit after deletion
            }

            prev = current;
            current = current->next;
        }
    }

    // Function to delete a vector from the linked list based on its index
void deleteVectorByIndex(int index) {
    if (index < 0) {
        std::cerr << "Invalid index" << std::endl;
        return;
    }

    Node* current = head;
    Node* prev = nullptr;
    int currentIndex = 0;

    while (current && currentIndex != index) {
        prev = current;
        current = current->next;
        currentIndex++;
    }

    if (current) {
        if (prev) {
            prev->next = current->next;
        } else {
            // If the first node is being deleted, update head
            head = current->next;
        }

        // If the last node is being deleted, update tail
        if (current == tail) {
            tail = prev;
        }

        delete current;
        size--;  // Decrement the size when a node is deleted
    } else {
        std::cerr << "Index out of bounds" << std::endl;
    }
}

    // Function to access a vector in the linked list based on its index
    Node* getNodeByIndex(int index) const {
        if (index < 0) {
            std::cerr << "Invalid index" << std::endl;
            return nullptr;
        }

        Node* current = head;
        int currentIndex = 0;

        while (current && currentIndex != index) {
            current = current->next;
            currentIndex++;
        }

        if (current) {
            return current;
        } else {
            std::cerr << "Index out of bounds" << std::endl;
            return nullptr;
        }
    }

    // Function to modify a vector in the linked list based on its index
    void modifyNodeByIndex(int index, const std::vector<int>& newData, int newPrice, const std::string& newInpline, char newMode) {
        Node* nodeToModify = getNodeByIndex(index);
        if (nodeToModify) {
            nodeToModify->data = newData;
            nodeToModify->price = newPrice;
            nodeToModify->inpline = newInpline;
            nodeToModify->mode = newMode;
        }
    }

    // Function to get the current size of the linked list
    int getSize() const {
        return size;
    }

    // Function to print the linked list in the order it was added
    void printListInOrder() const {
        Node* current = head;
        while (current) {
            // Print the vector elements
            for (int element : current->data) {
                std::cout << element << " ";
            }
            std::cout << "(Price: " << current->price << ", Inpline: " << current->inpline << ", Mode: " << current->mode << ") -> ";

            current = current->next;
        }
        std::cout << "nullptr" << std::endl;
    }
};

#endif // LINKED_LIST_H

#ifndef LINKED_LIST3_H
#define LINKED_LIST3_H

struct Node3 {
    std::vector<int> data;
    int price;
    std::string inpline;
    char mode;
    int quantity;  // New addition: an integer quantity
    Node3* next;

    // Constructor to initialize a node with data, a default price of 0, an empty inpline, 'N' as the default mode, and a default quantity of 0
    Node3(const std::vector<int>& data, int price = 0, const std::string& inpline = "", char mode = 'N', int quantity = 0)
        : data(data), price(price), inpline(inpline), mode(mode), quantity(quantity), next(nullptr) {}
};

class LinkedList3 {
private:
    Node3* head;
    Node3* tail;
    int size;

public:
    // Constructor to initialize an empty linked list
    LinkedList3() : head(nullptr), tail(nullptr), size(0) {}

    // Destructor to free memory when the linked list is destroyed
    ~LinkedList3() {
        while (head) {
            Node3* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Function to add a vector with a price, inpline, mode, and quantity to the end of the linked list
    void addVector(const std::vector<int>& data, int price = 0, const std::string& inpline = "", char mode = 'N', int quantity = 0) {
        Node3* newNode = new Node3(data, price, inpline, mode, quantity);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        size++;
    }

    // Function to delete a vector from the linked list based on its content
    void deleteVector(const std::vector<int>& data) {
        Node3* current = head;
        Node3* prev = nullptr;

        while (current) {
            if (current->data == data) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    head = current->next;
                }

                delete current;
                size--;
                return;
            }

            prev = current;
            current = current->next;
        }
    }

    // Function to delete a vector from the linked list based on its index
    void deleteVectorByIndex(int index) {
        if (index < 0) {
            std::cerr << "Invalid index" << std::endl;
            return;
        }

        Node3* current = head;
        Node3* prev = nullptr;
        int currentIndex = 0;

        while (current && currentIndex != index) {
            prev = current;
            current = current->next;
            currentIndex++;
        }

        if (current) {
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }

            if (current == tail) {
                tail = prev;
            }

            delete current;
            size--;
        } else {
            std::cerr << "Index out of bounds" << std::endl;
        }
    }

    // Function to access a vector in the linked list based on its index
    Node3* getNodeByIndex(int index) const {
        if (index < 0) {
            std::cerr << "Invalid index" << std::endl;
            return nullptr;
        }

        Node3* current = head;
        int currentIndex = 0;

        while (current && currentIndex != index) {
            current = current->next;
            currentIndex++;
        }

        if (current) {
            return current;
        } else {
            std::cerr << "Index out of bounds" << std::endl;
            return nullptr;
        }
    }

    // Function to modify a vector in the linked list based on its index
    void modifyNodeByIndex(int index, const std::vector<int>& newData, int newPrice, const std::string& newInpline, char newMode, int newQuantity) {
        Node3* nodeToModify = getNodeByIndex(index);
        if (nodeToModify) {
            nodeToModify->data = newData;
            nodeToModify->price = newPrice;
            nodeToModify->inpline = newInpline;
            nodeToModify->mode = newMode;
            nodeToModify->quantity = newQuantity;
        }
    }

    // Function to get the current size of the linked list
    int getSize() const {
        return size;
    }

    // Function to print the linked list in the order it was added
    void printListInOrder() const {
        Node3* current = head;
        while (current) {
            // Print the vector elements
            for (int element : current->data) {
                std::cout << element << " ";
            }
            std::cout << "(Price: " << current->price << ", Inpline: " << current->inpline << ", Mode: " << current->mode << ", Quantity: " << current->quantity << ") -> ";

            current = current->next;
        }
        std::cout << "nullptr" << std::endl;
    }
};

#endif // LINKED_LIST3_H
