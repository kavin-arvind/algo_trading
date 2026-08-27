// gets data through a reciver and identifies profitable trades.

#include "receiver.h"
#include "stock.h"

class Trader {
    private:
        std::string residue_message; // Unprocessed message remaining
        void processStock(Stock& stock) {
            std::cout << stock << "\n";
        }
    public:
        Trader() :
            residue_message("")
        {
            
        }
        void digestAndProcess(std::string& message) {
            residue_message += message;
            Stock::tokeniserResetTo(0);
            while(Stock::tokeniserPosition() < message.size()) {
                // Get stocks using tokeniser;
                int starting_postion = Stock::tokeniserPosition();
                if(Stock::tokeniser(residue_message, '#') == "") {
                    // see this case later
                    residue_message = std::string(residue_message.begin() + starting_postion, residue_message.end());
                    if(residue_message == "$") {
                        throw std::runtime_error("End Reached");
                    }
                    break;
                }
                // We've received a full line and we can process it into a full stock.
                Stock::tokeniserResetTo(starting_postion);
                std::string stock_name = Stock::tokeniser(residue_message, ' ');
                std::string price = Stock::tokeniser(residue_message, ' ');
                std::string tradeAction = Stock::tokeniser(residue_message, '#');
                Stock currStock(stock_name, tradeAction, price);
                processStock(currStock);
            }
        }
};

int main(int argc, char** argv) {
    try {
        Receiver receiver;
        Trader trader;
        while(true) {
            std::string message = receiver.readClient();
            if(message.size() == 0) continue;
            trader.digestAndProcess(message);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}