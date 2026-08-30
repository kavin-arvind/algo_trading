#include "trader.cpp"

class TraderPhase1 : public Trader {
    std::map<Stock::stock_name_type, Stock::price_type> seenStocks;
    std::set<Stock> boughtStocks;
    protected:
        void processStock(Stock& s) override {
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
};

int main(int argc, char** argv) {
    try {
        Receiver receiver;
        TraderPhase1 trader;
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