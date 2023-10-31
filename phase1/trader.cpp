#include "receiver.h"

#include <map>
std::string notrade = "No trade";

std::map <std::string, int> b_qoute; // {stock_name, value} best quote which is not cancelled
std::map <std::string, int> s_qoute; // {stock_name, value} """

std::map <std::string, int> stocks; // {stock_name, value}


std::string tokenizer(std::string txt, char l){ // delimiter = l
    static int k=0;
    if(txt == "reset"){k=0; return "";}
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
    std::string message;    

    
    while(true){
        message = rcv.readIML();
        if(message.size() ==0 || message[0] == '\0'){continue;}
        tokenizer("reset",'#');
        // inside for each message
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

            int price;
            try{
                price = stoi(price_str);
            }
            catch (const std::invalid_argument& e){
                std::cerr << "Invalid argument: " << price_str << std::endl;
                price = 0;
            }

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
                    if (b_qoute.find(stock_name) != b_qoute.end()){ // cancelling due to same price b and s
                        b_qoute.erase(stock_name);
                        out += notrade;
                    }
                    else{
                        if(price < stocks[stock_name]){ // quote accepted
                            out = out + stock_name +" "+ price_str + " " + "b";
                            stocks[stock_name] = price;
                        }
                        else{ // quote not accepted
                            if (s_qoute.find(stock_name) != s_qoute.end()){
                                if (s_qoute[stock_name] > price ) {s_qoute[stock_name] = price;}
                            }
                            else {s_qoute[stock_name] = price;}
                            out += notrade;
                        }                
                    }
                }
                else if(mode == 'b'){
                    if (s_qoute.find(stock_name) != s_qoute.end()){ // cancelling due to same price b and s
                        s_qoute.erase(stock_name);
                        out += notrade;
                    }
                    else{
                        if(price > stocks[stock_name]){ // quote accepted
                            out = out + stock_name +" "+ price_str + " " + "s";
                            stocks[stock_name] = price;
                        }
                        else{ // quote not accepted
                            if (b_qoute.find(stock_name) != b_qoute.end()){
                                if (b_qoute[stock_name] < price ) {b_qoute[stock_name] = price;}
                            }
                            else {b_qoute[stock_name] = price;}
                            out += notrade;
                        }                
                    }
                }
                else{
                    out+="invalid request";
                }
            }

            // ________________________________________

            // out+=  "\r\n";
            // for(int i=0;i<out.size();i++){
            //     if(out[i]=='\0'){continue;}
            //     std::cout<<out[i];
            // }
            std::cerr<< "printing something"<<std::endl;
            std::cout<<out<<std::endl;

        }
        // std::cout<< message << std::endl;

        // ____________ for every message
        if(message[message.size()-1] == '$'){ break;}
    }

// _________________________________________________________
    // std::cout<< out;
    // rcv.terminate();
    return 0;
}
