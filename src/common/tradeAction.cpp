#include "common/tradeAction.h"

TradeAction stringToTradeAction(const std::string& str) {
    if(str == "b") return TradeAction::BUY;
    if(str == "s") return TradeAction::SELL;
    return TradeAction::UD;
}