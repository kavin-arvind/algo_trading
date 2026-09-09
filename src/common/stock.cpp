#include "common/stock.h"

Stock::Stock(stock_name_type _stock_name, TradeAction _tradeAction, price_type _price) :
    stock_name(_stock_name),
    tradeAction(_tradeAction),
    price(_price)
    {
    }

Stock::Stock() :
    stock_name("UD"),
    tradeAction(TradeAction::UD),
    price(0)
    {
    }

Stock::Stock(const Stock& s) :
    stock_name(s.stock_name),
    tradeAction(s.tradeAction),
    price(s.price)
    {
    }
bool Stock::operator<(const Stock& other) const {
    return stock_name < other.stock_name;
}
bool Stock::operator==(const Stock& other) const {
    return stock_name == other.stock_name;
}
std::ostream &operator<<(std::ostream& os, const Stock& stock) {
    os << stock.stock_name << " ";
    std::string temp[3] = {"Buy", "Sell", "UD"};
    os << temp[(int)stock.tradeAction] << " ";
    os << stock.price << " ";
    return os;
}

