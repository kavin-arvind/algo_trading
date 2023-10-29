#include "receiver.h"

#include <map>
std::string notrade = "No trade";

std::string tokenizer(std::string txt, char l){ // delimiter = l
    static int k=0;
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
    if(txt[k]=='\0'){return "";}
    k++;
    return temp;
}

int main() {

    Receiver rcv;
    // sleep(5);
    std::string message = rcv.readIML();    

    std::map <std::string, int> stocks; // {stock_name, value}
    

    while (true){

        std::string out;

        // stock_name represents the string containing stock name
        // price give the price in string which needed to be converted to int
        // mode gives s if its sell and b if its buy

        std::string line = tokenizer(message, '#');

        if (line.empty()){break;}

        int i=0;
        std::string stock_name;
        while(true){
            if(line[i]=='\0'){break;}
            if(line[i]==' '){break;}
            stock_name += line[i];
            i++;
        }
        i++;

        std::string price_str;
        while(true){
            if(line[i]=='\0'){break;}
            if(line[i]==' '){break;}
            price_str += line[i];
            i++;
        }
        i++;
        
        char mode = line[i];
        int price = stoi(price_str);
        // actual working 

        if(stocks.find(stock_name)==stocks.end()){ // if stock encountered for the first time
            stocks[stock_name] = price;
            if(mode == 's'){
                out = out + stock_name +" "+ price_str + " " + "b";
            }
            else if(mode=='b'){
                out = out + stock_name +" "+ price_str + " " + "s";
            }
            else{
                out+="invalid request";
            }

        }

        else{ // if stock ALREADY encountered
            if(mode == 's'){
                if(price < stocks[stock_name]){
                    out = out + stock_name +" "+ price_str + " " + "b";
                    stocks[stock_name] = price;
                }
                else{
                    out += notrade;
                }                
            }
            else if(mode=='b'){
                if(price > stocks[stock_name]){
                    out = out + stock_name +" "+ price_str + " " + "s";
                    stocks[stock_name] = price;
                }
                else{
                    out += notrade;
                }                
            }
            else{
                out+="invalid request";
            }
        }

        // ________________________________________

        out+=  "\r\n";
        // for(int i=0;i<out.size();i++){
        //     if(out[i]=='\0'){continue;}
        //     std::cout<<out[i];
        // }
        std::cout<<out;

    }




// _________________________________________________________
    // std::cout<< out;
    return 0;
}
