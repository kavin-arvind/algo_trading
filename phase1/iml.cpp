// Takes input of a file, reads every line and gives it to trader logic.
// This is a market simulator

#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "properties.h"

class TraderConnector {
    private:
        int clientsocketfd;
    public:
        TraderConnector(sockaddr_in traderaddr) {
            clientsocketfd = socket(AF_INET, SOCK_STREAM, 0);
            if(clientsocketfd == -1){
                throw std::runtime_error("Error creating socket to connect to trader");
            }
            if (connect(clientsocketfd, (sockaddr*) &traderaddr, sizeof(traderaddr)) == -1) {
                throw std::runtime_error("Error connecting to trader");
            }
            std::cout << "Connected to Trader\n";
        }
        int sendMessageToTrader(std::string &message) {
            // returns the bytes sent
            return send(clientsocketfd, message.c_str(), message.length(), 0);
        }
        ~TraderConnector() {
            close(clientsocketfd);
        }
};

int main(int argc, char ** argv){
    try {
        if(argc < 2) {
            throw std::runtime_error("Trading history not provided");
        }
        std::string filepath = argv[1];

        // Trader's receiver settings
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(TraderProperties::TRADER_PORT);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        TraderConnector traderconnect(serverAddress);

        std::ifstream datafile(filepath);
        std::string message;

        while(std::getline(datafile, message)) {
            // send message to Trader's reciever
            int bytessent = traderconnect.sendMessageToTrader(message);
            if(bytessent == -1) {
                throw std::runtime_error("Error while sending message from iml to trader");
            }
        }
        
        message = "$";
        int bytessent = traderconnect.sendMessageToTrader(message);
        if(bytessent == -1) {
            throw std::runtime_error("Error while sending message from iml to trader");
        }

        return 0;
    } catch (const std::exception& e){
        std::cerr << e.what() << "\n";
        return 1;
    }
}