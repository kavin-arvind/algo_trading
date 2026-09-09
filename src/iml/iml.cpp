#include "iml/iml.h"

Iml::Iml(sockaddr_in traderaddr) {
    clientsocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientsocketfd == -1){
        throw std::runtime_error("Error creating socket to connect to trader");
    }
    if (connect(clientsocketfd, (sockaddr*) &traderaddr, sizeof(traderaddr)) == -1) {
        throw std::runtime_error("Error connecting to trader");
    }
    std::cout << "Connected to Trader\n";
}

int Iml::sendMessageToTrader(std::string &message) {
    // returns the bytes sent
    return send(clientsocketfd, message.c_str(), message.length(), 0);
}

Iml::~Iml() {
    close(clientsocketfd);
}