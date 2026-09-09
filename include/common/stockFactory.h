#pragma once

#include "common/stock.h"
#include "common/tokeniser.h"
#include "common/stockFactory.h"
#include "trader/trader.h"

class StockFactory {
    private:
        int mode;
        std::string residue_message;
    public:
        StockFactory(int _mode) :
            mode(_mode)
        {}

        void digestAndCallTrader(std::string&, Trader*);
        Stock stockConstructor(const std::string& message, size_t pos);
        /*
        Constructs a Stock of template - ASML 650 s#
        from the given 
        @param message - input string
        @param pos Reads message starting from pos
        */
        Stock simpleConstruction(const std::string& message, size_t pos);
};