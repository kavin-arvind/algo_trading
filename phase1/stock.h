#include <iostream>
#include <string.h>

enum class TradeAction {
    BUY = 0,
    SELL = 1,
    UD = 2 // Undefined
};

class Stock {
    private:
        std::string stock_name;
        TradeAction tradeAction;
        long long int price;
        /*
        Finds the next next tokens one by one. Reset the token if required
        */
    public:
        Stock(std::string _stock_name, std::string _tradeAction, std::string _price) :
            stock_name(_stock_name),
            tradeAction((_tradeAction == "b" ? TradeAction::BUY : (_tradeAction == "s" ? TradeAction::SELL : TradeAction::UD))),
            price(std::stol(_price))
            {
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