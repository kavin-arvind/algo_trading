#include "iml/iml.h"
#include "common/properties.h"

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