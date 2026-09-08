#include "trader/traderPhase1.h"

void TraderPhase1::processStock(Stock& s) {
    auto seenStockPtr = seenStocks.find(s.stock_name);
    if(seenStockPtr == seenStocks.end()) {
        // new stock has arrived
        seenStocks[s.stock_name] = s.price;

        Stock proposedStock(s);
        if(s.tradeAction == TradeAction::BUY) {
            proposedStock.tradeAction = TradeAction::SELL;
        } else if(s.tradeAction == TradeAction::SELL) {
            proposedStock.tradeAction = TradeAction::BUY;
        }
        if(!orderStock(proposedStock)) {
            std::cerr << "Couldn't order proposed stock - " << proposedStock << "\n";
        }
        return;
    }
    // stock is already seen
    if(s.price < seenStockPtr->second) {
        // We shall buy if they sell
        if(s.tradeAction == TradeAction::SELL) {
            // They too want to sell
            seenStockPtr->second = s.price;
            Stock proposedStock(s);
            proposedStock.tradeAction = TradeAction::BUY;
            if(!orderStock(proposedStock)) {
                std::cerr << "Couldn't order proposed stock - " << proposedStock << "\n";
            }
            return;
        }
    } else if (s.price > seenStockPtr->second) {
        // We shall sell if they buy
        if(s.tradeAction == TradeAction::BUY) {
            // They too want to sell
            seenStockPtr->second = s.price;
            Stock proposedStock(s);
            proposedStock.tradeAction = TradeAction::SELL;
            if(!orderStock(proposedStock)) {
                std::cerr << "Couldn't order proposed stock - " << proposedStock << "\n";
            }
            return;
        }
    }
    noTrade();
    return;
}