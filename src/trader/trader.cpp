#include "trader/trader.h"

void Trader::processStock(Stock& stock) {
    std::cout << stock << "\n";
}

Trader::Trader() :
    residue_message("")
{
    
}
void Trader::digestAndProcess(std::string& message) {
    residue_message += message;
    Tokeniser::tokeniserResetTo(0);
    while(true) {
        // Get stocks using tokeniser;
        if(residue_message.find('#', Tokeniser::tokeniserPosition()) == std::string::npos) {
            // We don't have a complete line
            residue_message = std::string(residue_message.begin() + Tokeniser::tokeniserPosition(), residue_message.end());
            if(residue_message == "$") {
                throw std::runtime_error("End Reached");
            }
            break;
        }
        // We've received a full line and we can process it into a full stock.
        std::string stock_name = Tokeniser::tokeniser(residue_message, ' ');
        std::string price = Tokeniser::tokeniser(residue_message, ' ');
        std::string tradeAction = Tokeniser::tokeniser(residue_message, '#');
        Stock currStock(stock_name, tradeAction, price);
        processStock(currStock);
    }
}


bool Trader::orderStock(Stock s) {
    std::cout << s << "\n";
    return true;
}
void Trader::noTrade() {
    std::cout << "No Trade" <<"\n";
}
