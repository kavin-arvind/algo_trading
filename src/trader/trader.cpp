#include "trader/trader.h"
#include "common/stockFactory.h"

Trader::Trader() {}

void Trader::processStock(Stock& stock) {
    std::cout << stock << "\n";
}

bool Trader::orderStock(Stock s) {
    std::cout << s << "\n";
    return true;
}

void Trader::noTrade() {
    std::cout << "No Trade" <<"\n";
}
