#pragma once

#include <iostream>
#include "common/tradeAction.h"

class Stock {
    public:
        using stock_name_type = std::string;
        using price_type = long long int;
        stock_name_type stock_name;
        TradeAction tradeAction;
        price_type price;

        Stock(std::string _stock_name, std::string _tradeAction, std::string _price);
        
        Stock(const Stock& s);
        bool operator<(const Stock& other) const;
        bool operator==(const Stock& other) const;
        friend std::ostream &operator<<(std::ostream& os, const Stock& stock);
};

