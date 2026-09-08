#pragma once

#include "trader/trader.h"

class TraderPhase1 : public Trader {
    private:
        std::map<Stock::stock_name_type, Stock::price_type> seenStocks;
        std::set<Stock> boughtStocks;
    protected:
        void processStock(Stock& s) override;
};