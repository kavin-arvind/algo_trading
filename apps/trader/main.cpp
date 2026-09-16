#include "trader/traderFactory.h"
#include "common/stockFactory.h"

int main(int argc, char** argv) {
    try {
        Receiver receiver;
        std::unique_ptr<Trader> trader = std::make_unique<TraderPhase1> ();
        StockFactory stockFactory(1);
        while(true) {
            std::string message = receiver.readClient();
            if(message.size() == 0) continue;
            stockFactory.digestAndCallTrader(message, *trader);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}