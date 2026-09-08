#pragma once

#include "common/properties.h"
#include "common/stock.h"
#include "common/tokeniser.h"

#include <bits/stdc++.h>
#include "trader/receiver.h"

class Trader {
    private:
        std::string residue_message;
    protected:
        virtual void processStock(Stock&);
    public:
        Trader();
        void digestAndProcess(std::string&);
        /*
        Give a proposed stock to propose it in the market
        */
        bool orderStock(Stock s);
        void noTrade();
};