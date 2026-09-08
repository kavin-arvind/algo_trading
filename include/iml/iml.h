#pragma once

// Takes input of a file, reads every line and gives it to trader logic.
// This is a market simulator

#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>

class TraderConnector {
    private:
        int clientsocketfd;
    public:
        TraderConnector(sockaddr_in traderaddr);
        int sendMessageToTrader(std::string &message);
        ~TraderConnector();
};