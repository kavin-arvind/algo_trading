#include "common/stockFactory.h"

void StockFactory::digestAndCallTrader(std::string& message, Trader* trader) {
    residue_message += message;
    Tokeniser::tokeniserResetTo(0);
    while(true) {
        // Get stocks using tokeniser;
        size_t found_pos = residue_message.find('#', Tokeniser::tokeniserPosition());
        if(found_pos == std::string::npos) {
            // We don't have a complete line
            residue_message = std::string(residue_message.begin() + Tokeniser::tokeniserPosition(), residue_message.end());
            if(residue_message == "$") {
                throw std::runtime_error("End Reached");
            }
            break;
        }
        // We've received a full line and we can process it into a full stock.
        Stock currStock = StockFactory::simpleConstruction(residue_message, Tokeniser::tokeniserPosition());
        Tokeniser::tokeniserResetTo(found_pos+1);
        trader->processStock(currStock);
    }
}

Stock StockFactory::stockConstructor(const std::string& message, size_t pos) {
    if(mode == 1) {
        return simpleConstruction(message, pos);
    }
    return Stock();
}

Stock StockFactory::simpleConstruction(const std::string& message, size_t pos) {
    size_t tokeniserPos = Tokeniser::tokeniserPosition();
    Tokeniser::tokeniserResetTo(pos);
    std::string stock_name = Tokeniser::tokeniser(message, ' ');
    std::string price = Tokeniser::tokeniser(message, ' ');
    std::string tradeAction = Tokeniser::tokeniser(message, '#');
    Tokeniser::tokeniserResetTo(tokeniserPos);
    return Stock(stock_name, stringToTradeAction(tradeAction), stol(price));
}