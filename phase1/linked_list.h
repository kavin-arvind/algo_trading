#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <vector>
#include <string>
#include <iostream>

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
