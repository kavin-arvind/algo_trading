#include "receiver.h"

// #include <map>
#include "map.h"

std::string notrade = "No trade";
char newline = '\n';
std::string str_newline = "\n";
AVLMap b_qoute; // {stock_name, value} best quote which is not cancelled
AVLMap s_qoute; // {stock_name, value} """

// AVLMap stocks; // {stock_name, predicted value} 

AVLMap b_best; // {stock_name, predicted value} stores best price of a stock bought ever
AVLMap s_best; // {stock_name, predicted value} stores best price of a stock bought ever


std::string tokenizer(std::string txt, char l){ // delimiter = l
    static int k=0;
    if(txt == "__reset__"){k=0; return "";}
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
    if(txt == "__reset__"){k=0; return "";}
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
        tokenizer("__reset__",'#');
// usleep(100000);
        // inside for each message
        while (true){

            std::string out="";

            // stock_name represents the string containing stock name
            // price give the price in string which needed to be converted to int
            // mode gives s if its sell and b if its buy
            
            std::string line = tokenizer(message, '#');

            if (line.empty()){break;}
            tokenizer_in_msg("__reset__", ' ');

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

            if(mode == 's'){
                if (b_qoute.containsKey(stock_name) && b_qoute.getValue(stock_name) == price){ // cancelling due to same price b and s
                    b_qoute.remove(stock_name);
                    out += notrade;
                }
                else{
                    if(s_best.containsKey(stock_name)){ // already sold
                        if(price < s_best.getValue(stock_name)){ // quote accepted
                            s_best.update(stock_name, price);
                            out = out + stock_name +" "+ price_str + " " + "b";
                        }
                        else{ // quote not accepted
                            if(s_qoute.containsKey(stock_name)){
                                if (price < s_qoute.getValue(stock_name) ) {s_qoute.update(stock_name,price);}
                            }
                            else{
                                s_qoute.insert(stock_name,price);
                            }
                            out = out + notrade;
                        }
                    }
                    else if(b_best.containsKey(stock_name)){
                        if(price < b_best.getValue(stock_name)){ // quote accepted
                            s_best.insert(stock_name, price);
                            out = out + stock_name +" "+ price_str + " " + "b";
                        }
                        else{ // quote not accepted
                            if(s_qoute.containsKey(stock_name)){
                                if (price < s_qoute.getValue(stock_name)) {s_qoute.update(stock_name,price);}
                            }
                            else{
                                s_qoute.insert(stock_name,price);
                            }
                            out = out + notrade;
                        }
                    }
                    else{ // first time stock encountered
                        s_best.insert(stock_name, price);
                        out = out + stock_name +" "+ price_str + " " + "b";
                    }
                }
            }

            else if(mode == 'b'){
                if (s_qoute.containsKey(stock_name) && s_qoute.getValue(stock_name) == price){ // cancelling due to same price b and s
                    s_qoute.remove(stock_name);
                    out += notrade;
                }
                else{
                    if(b_best.containsKey(stock_name)){ // already sold
                        if(price > b_best.getValue(stock_name)){ // quote accepted
                            b_best.update(stock_name, price);
                            out = out + stock_name +" "+ price_str + " " + "s";
                        }
                        else{ // quote not accepted
                            if(b_qoute.containsKey(stock_name)){
                                if ( price > b_qoute.getValue(stock_name)) {b_qoute.update(stock_name,price);}
                            }
                            else{
                                b_qoute.insert(stock_name,price);
                            }
                            out = out + notrade;
                        }
                    }
                    else if(s_best.containsKey(stock_name)){
                        if(price > s_best.getValue(stock_name)){ // quote accepted
                            b_best.insert(stock_name, price);
                            out = out + stock_name +" "+ price_str + " " + "s";
                        }
                        else{ // quote not accepted
                            if(b_qoute.containsKey(stock_name)){
                                if (price > b_qoute.getValue(stock_name) ) {b_qoute.update(stock_name,price);}
                            }
                            else{
                                b_qoute.insert(stock_name,price);
                            }
                            out = out + notrade;
                        }
                    }
                    else{ // first time stock encountered
                        b_best.insert(stock_name, price);
                        out = out + stock_name +" "+ price_str + " " + "s";
                    }
                }
            }


            // ________________________________________
            // out+=  "\n";
            // for(int i=0;i<out.size();i++){
            //     if(out[i]=='\0'){continue;}
            //     std::cout<<out[i];
            // }
            std::cout<<out<<std::endl;
        }
        // std::cout<< message << std::endl;

        // ____________ for every message
        if(message[message.size()-1] == '$'){ break;}
    }

// _________________________________________________________
    // std::cout<< out;
    rcv.terminate();
    return 0;
}
