#include "header.h"

int main() {
    AVLMap avlMap;
    AVLMap2 avlMap2;

    // Test AVLMap
    avlMap.insert("AAPL", {1, 2, 3});
    avlMap.insert("GOOGL", {4, 5, 6});

    std::cout << "AVLMap Test:" << std::endl;
    std::cout << "AAPL value: ";
    for (int val : avlMap.getValue("AAPL")) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "GOOGL value: ";
    for (int val : avlMap.getValue("GOOGL")) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    avlMap.remove("AAPL");

    std::cout << "AAPL present: " << (avlMap.containsKey("AAPL") ? "true" : "false") << std::endl;

    // Test AVLMap2
    Heap* heap1 = new Heap();
    Heap* heap2 = new Heap();

    Node node1("AAPL", "GOOGL", 123, 456, 'b', 10, 100);
    Node node2("GOOGL", "AAPL", 789, 987, 's', 5, 200);

    heap1->insert(&node1);
    heap2->insert(&node2);

    avlMap2.insert("AAPL", heap1);
    avlMap2.insert("GOOGL", heap2);

    std::cout << "\nAVLMap2 Test:" << std::endl;
    std::cout << "AAPL value: ";
    Node* topNode = avlMap2.getValue("AAPL")->peek();
    if (topNode) {
        std::cout << "Broker: " << topNode->broker << ", Price: " << topNode->price << std::endl;
    } else {
        std::cout << "No value for AAPL" << std::endl;
    }

    std::cout << "GOOGL value: ";
    topNode = avlMap2.getValue("GOOGL")->peek();
    if (topNode) {
        std::cout << "Broker: " << topNode->broker << ", Price: " << topNode->price << std::endl;
    } else {
        std::cout << "No value for GOOGL" << std::endl;
    }

    avlMap2.remove("AAPL");

    std::cout << "AAPL present: " << (avlMap2.containsKey("AAPL") ? "true" : "false") << std::endl;

    return 0;
}
