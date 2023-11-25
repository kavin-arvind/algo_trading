#include "linked_list.h"

int main() {
    // Create a LinkedList
    LinkedList myList;

    // Add vectors with prices, inplines, and modes
    std::vector<int> demo = {1,2,3};
    myList.addVector(demo, 10, "abc", 'A');
    myList.addVector({4, 5, 6}, 20, "xyz", 'B');
    myList.addVector({7, 8, 9}, 30, "123", 'C');

    // Print the initial list
    std::cout << "Initial List:\n";
    myList.printListInOrder();
    demo[0] = 100; std::cout<<"after modification"<<demo[0]<<std::endl;
    myList.printListInOrder();
    std::cout << "Size: " << myList.getSize() << "\n\n";

    // Delete a vector by content
    myList.deleteVector({4, 5, 6});
    std::cout << "List after deleting {4, 5, 6}:\n";
    myList.printListInOrder();
    std::cout << "Size: " << myList.getSize() << "\n\n";

    // Delete a vector by index
    myList.deleteVectorByIndex(1);
    std::cout << "List after deleting index 1:\n";
    myList.printListInOrder();
    std::cout << "Size: " << myList.getSize() << "\n\n";

    // Modify a vector by index
    myList.modifyNodeByIndex(0, {10, 20, 30}, 50, "modified", 'M');
    std::cout << "List after modifying index 0:\n";
    myList.printListInOrder();
    std::cout << "Size: " << myList.getSize() << "\n\n";

    // Access a vector by index
    Node* accessedNode = myList.getNodeByIndex(0);
    if (accessedNode) {
        std::cout << "Accessed Node at index 0:\n";
        std::cout << "Data: ";
        for (int element : accessedNode->data) {
            std::cout << element << " ";
        }
        std::cout << "\nPrice: " << accessedNode->price << "\nInpline: " << accessedNode->inpline << "\nMode: " << accessedNode->mode << "\n\n";
    }

    return 0;
}
