#pragma once

#include <iostream>

enum class TradeAction {
    BUY = 0,
    SELL = 1,
    UD = 2 // Undefined
};

TradeAction stringToTradeAction(const std::string& );