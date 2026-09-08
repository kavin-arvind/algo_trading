#pragma once

#include <bits/stdc++.h>
#include <format>
#include <sys/socket.h>
#include <netinet/in.h>
#include "common/properties.h"

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
        auto acceptNewConnection();
    public:
        Receiver();
        std::string readClient();
};