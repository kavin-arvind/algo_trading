#include "receiver.h"

#include <map>
std::string notrade = "No trade";
char newline = '\n';
std::map <std::string, int> b_qoute; // {stock_name, value} best quote which is not cancelled
std::map <std::string, int> s_qoute; // {stock_name, value} """

std::map <std::string, int> stocks; // {stock_name, predicted value} 


std::string tokenizer(std::string txt, char l){ // delimiter = l
    static int k=0;
    if(txt == "reset"){k=0; return "";}
    std::string temp;
    while (true){
        if (txt[k]=='\0' || txt[k] == '$') {break;}
        if (txt[k]==l) {break;}
        if(txt[k] == newline){k++;continue;} // if its \n, then basically ignore it and increment k.
        temp += txt[k];
        k++;
    }
    k++;
    // if(txt[k]=='\0'){return "";}
    // k++;
    return temp;
}

std::string tokenizer_in_msg(std::string txt, char l){ // delimiter = l
    static int k=0;
    if(txt == "reset"){k=0; return "";}
    std::string temp;
    while (true){
        if (txt[k]=='\0' || txt[k] == '$') {break;}
        if (txt[k]==l) {break;}
        if(txt[k] == newline){k++;continue;} // if its \n, then basically ignore it and increment k.
        temp += txt[k];
        k++;
    }
    k++;
    // if(txt[k]=='\0'){return "";}
    // k++;
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
// usleep(100000);
        // inside for each message
        while (true){

            std::string out;

            // stock_name represents the string containing stock name
            // price give the price in string which needed to be converted to int
            // mode gives s if its sell and b if its buy
            
            std::string line = tokenizer(message, '#');

            if (line.empty()){break;}
            tokenizer_in_msg("reset", ' ');

            std::string stock_name = tokenizer_in_msg(line, ' '); if(stock_name.size()==0){break;}

            std::string price_str = tokenizer_in_msg(line, ' '); if(price_str.size()==0){break;}

            char mode;
            std::string mode_str = tokenizer_in_msg(line, '#');
            if(mode_str == "s"){mode = 's';}
            else if(mode_str == "b"){mode = 'b';}
            else{break;}

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
                    if (b_qoute.find(stock_name) != b_qoute.end() && b_qoute[stock_name] == price){ // cancelling due to same price b and s
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
                    if (s_qoute.find(stock_name) != s_qoute.end() && s_qoute[stock_name] == price){ // cancelling due to same price b and s
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
            }

            // ________________________________________
            out+=  "\n";
            // for(int i=0;i<out.size();i++){
            //     if(out[i]=='\0'){continue;}
            //     std::cout<<out[i];
            // }
            std::cout<<out;
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
