#include <bits/stdc++.h>
#include <format>
#include <sys/socket.h>
#include <netinet/in.h>
#include "properties.h"

/*
Receives Trades from only 1 client (the first client)
*/
class Receiver {
    private:
        int receiversocketfd;
        sockaddr_in serverAddress;
        int cliensocketfd;
        sockaddr_in clientAddress;
        static const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        /*
            Returns the socket and clientaddress of the newly accepted connection.
            Users must close this fd according to their needs and this class is not responsible for closing this fd        
        */
        auto acceptNewConnection() {          
            sockaddr_in clientAddress;
            socklen_t clientAddressLen = sizeof(clientAddress);
            int clsocketfd = accept(receiversocketfd, (sockaddr*) &clientAddress, &clientAddressLen);
            return std::pair<int, sockaddr_in> {clsocketfd, clientAddress};
        }
    public:
        Receiver() {
            receiversocketfd = socket(AF_INET, SOCK_STREAM, 0);
            if(receiversocketfd == -1) {
                throw std::runtime_error("Error while creating recieiver socket");
            }
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = INADDR_ANY;
            serverAddress.sin_port = htons(TraderProperties::TRADER_PORT);

            if(bind(receiversocketfd, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1){
                throw std::runtime_error(std::string("Error while binding to port ") + std::to_string(TraderProperties::TRADER_PORT));
            }

            if(listen(receiversocketfd, TraderProperties::MAX_LISTEN_QUEUE_SIZE) == -1) {
                throw std::runtime_error("Error Listening to new connections");
            }

            std::cout << "Receiver Listening on port " << TraderProperties::TRADER_PORT << "\n";
            std::tie(cliensocketfd, clientAddress) = acceptNewConnection();
            if(cliensocketfd == -1) {
                throw std::runtime_error("Error connecting to the first connection");
            }
            std::cout << "Connection established successfully" << "\n";
        }

        std::string readClient(){
            // usleep(100);
            int bytesRead = recv(cliensocketfd, buffer, BUFFER_SIZE - 1, 0);
            if (bytesRead == -1) {
                throw std::runtime_error("Error receiving data from sender.");
            }
            buffer[bytesRead] = '\0'; // Null-terminate the received data

            std::string message = buffer;
            return message;
        }
};