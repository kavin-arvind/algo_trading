#pragma once

#include "common/properties.h"
#include "common/stock.h"
#include "common/tokeniser.h"

#include <bits/stdc++.h>
#include "trader/receiver.h"

class Trader {
    public:
        Trader();
        /*
        Give a proposed stock to propose it in the market
        */
        virtual void processStock(Stock&);
        bool orderStock(Stock s);
        void noTrade();
};