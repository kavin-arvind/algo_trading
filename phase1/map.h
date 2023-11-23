#ifndef MAP_AVL_H
#define MAP_AVL_H

#include <string>

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
