#include "receiver.h"

// #include <map>
#include "map.h"
#include "linked_list.h"
#define vector_size 100
std::string notrade = "No trade";
char newline = '\n';
std::string str_newline = "\n";
AVLMap b_qoute; // {stock_name, value} best quote which is not cancelled
AVLMap s_qoute; // {stock_name, value} """

AVLMap stock_index; // from stock to index of it in the array
// AVLMap stocks; // {stock_name, predicted value} 
LinkedList input_lines; // llist of inputted lines


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

bool compareVectors(const std::vector<int>& v1, const std::vector<int>& v2) {
    // Check if the size of the vectors is the same
    if (v1.size() != v2.size()) {
        return false;
    }

    // Use std::equal to compare the elements of the vectors
    return std::equal(v1.begin(), v1.end(), v2.begin());
}

// LETS GO 
//O(2^n)
void fgfg(char* s, vector<char> &res, int n){ 
        if (n == 0) { 
            for (auto i: res)  
              cout << i; 
            cout << "\n"; 
            return; 
              
        } 
         res.push_back(s[n - 1]); 
         f(s, res, n - 1); 
         res.pop_back();                     
         f(s, res, n - 1); 
    } 

//O(2^n)

void f(LinkedList &LL, int n, int* arrind){
    if ( n==0 ){
        LL.getVectorByIndex(n-1)
    }
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
            
            std::string line = tokenizer(message, '#');

            if (line.empty()){break;}
            tokenizer_in_msg("__reset__", ' ');

            // process input (one linear combo)

            int ite=0;
            std::vector<std::string> linelst;
            std::string lineobj = "";
            //remove leading whitespaces
            while(true){
                if(line[ite] == ' '){
                    ite++;
                }
                else{break;}
            }

            while(true){
                if (line[ite] == '\0'){
                    if (lineobj != ""){
                        linelst.push_back(lineobj); // push the str obj
                        lineobj = "";
                    }
                    break;
                } // to reach end of line

                if (line[ite] == ' '){
                    linelst.push_back(lineobj); // push the str obj
                    lineobj = "";
                    while(true){ // remove further whitespaces
                        if(line[ite] == ' '){
                            ite++;
                        }
                        else{break;}
                    }
                }

                lineobj += line[ite];
                ite++; 

            }

            // removing extra strings at the last

            std::string lastele = linelst.back();
            if (not (lastele=="b" || lastele=="s")){
                linelst.pop_back();
            }

            // processing line ends here

            if (linelst.size() < 2){break;}

            // mode is last element
            char mode;
            std::string mode_str = linelst.back();
            if(mode_str == "s"){mode = 's';}
            else if(mode_str == "b"){mode = 'b';}
            else{break;}

            // price is 2nd last element
            std::string price_str = linelst[linelst.size() - 2];
            int price;
            try{
                if(mode=='b')price = stoi(price_str);
                else if(mode=='s')price = (-1)*stoi(price_str);
            }
            catch (const std::invalid_argument& e){
                std::cerr << "Invalid argument: " << price_str << std::endl;
                price = 0;
            }

            // actual working 
            linelst.pop_back(); // popping b/s
            linelst.pop_back(); // popping price

            // process through the input vector
            std::vector<int> append_vector(vector_size);
            for(int i=0;i<vector_size;++i){ //initialize to 0
                append_vector[i]=0;
            }
            for (int i=0; i < linelst.size(); i=i+2){
                if(mode=='b'){// no problem.. we just append to the list
                    if(stock_index.containsKey(linelst[i])){
                        append_vector[stock_index.getValue(linelst[i])] = stoi(linelst[i+1]);
                    }
                    else{
                        int sz = stock_index.getSize();
                        stock_index.insert(linelst[i],sz);
                        append_vector[sz] = stoi(linelst[i+1]);
                    }
                }
                else if(mode=='s'){// take negative of everything
                    if(stock_index.containsKey(linelst[i])){
                        append_vector[stock_index.getValue(linelst[i])] = (-1)*stoi(linelst[i+1]);
                    }
                    else{
                        int sz = stock_index.getSize();
                        stock_index.insert(linelst[i],sz);
                        append_vector[sz] = (-1)*stoi(linelst[i+1]);
                    }
                }
            }

            //CANCELLATION LAWS
            bool flag1=false;
            for(int i=0;i < input_lines.getSize(); ++i){
                if(compareVectors(input_lines.getNodeByIndex(i)->data, append_vector)){
                    if(mode != input_lines.getNodeByIndex(i)->mode){
                        input_lines.deleteVectorByIndex(i);
                        flag1=true;
                        break;
                    }
                    else{
                        if(input_lines.getNodeByIndex(i)->price < price){
                            input_lines.getNodeByIndex(i)->price = price;
                            flag1=true;
                            break;
                        }
                    }
                }
            }
            if(flag1==false){input_lines.addVector(append_vector, price, line, mode);}

            

            // ________________________________________
            // out+=  "\n";
            // for(int i=0;i<out.size();i++){
            //     if(out[i]=='\0'){continue;}
            //     std::cout<<out[i];
            // }
            std::cout<<out<<"\r\n";
        }
        //input_lines.printListInOrder();
        // std::cout<< message << std::endl;



        // ____________ for every message
        if(message[message.size()-1] == '$'){ break;}
    }

// _________________________________________________________
    // std::cout<< out;
    rcv.terminate();
    return 0;
}
