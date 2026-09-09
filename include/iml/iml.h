#pragma once

// Takes input of a file, reads every line and gives it to trader logic.
// This is a market simulator

#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Iml {
    private:
        int clientsocketfd;
    public:
        Iml(sockaddr_in traderaddr);
        int sendMessageToTrader(std::string &message);
        ~Iml();
};