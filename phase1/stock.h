#include <iostream>
#include <string.h>

enum class TradeAction {
    BUY = 0,
    SELL = 1,
    UD = 2 // Undefined
};

class Stock {
    public:
        using stock_name_type = std::string;
        using price_type = long long int;
        stock_name_type stock_name;
        TradeAction tradeAction;
        price_type price;

        Stock(std::string _stock_name, std::string _tradeAction, std::string _price) :
            stock_name(_stock_name),
            tradeAction((_tradeAction == "b" ? TradeAction::BUY : (_tradeAction == "s" ? TradeAction::SELL : TradeAction::UD))),
            price(std::stol(_price))
            {
            }
        
        Stock(const Stock& s) :
            stock_name(s.stock_name),
            tradeAction(s.tradeAction),
            price(s.price)
            {
            }
        bool operator<(const Stock& other) const {
            return stock_name < other.stock_name;
        }
        bool operator==(const Stock& other) const {
            return stock_name == other.stock_name;
        }
        friend std::ostream &operator<<(std::ostream& os, const Stock& stock) {
            os << stock.stock_name << " ";
            std::string temp[3] = {"Buy", "Sell", "UD"};
            os << temp[(int)stock.tradeAction] << " ";
            os << stock.price << " ";
            return os;
        }
};

class Tokeniser {
    private:
        inline static int k = 0;
    public:
        static std::string tokeniser(std::string& txt, char l) {
            std::string temp;
            while (true){
                if (txt[k]=='\0') {
                    break;
                }
                if (txt[k]==l) {
                    break;
                }
                temp += txt[k];
                k++;
            }
            k++;
            if(txt[k-1]=='\0'){return "";}
            return temp;
        }
        inline static void tokeniserResetTo(int pos = 0) {
            k = pos;
        }
        inline static int tokeniserPosition() {
            return k;
        }
};