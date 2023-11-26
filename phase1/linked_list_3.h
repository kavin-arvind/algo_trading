#ifndef LINKED_LIST3_H
#define LINKED_LIST3_H

#include <vector>
#include <string>
#include <iostream>

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
