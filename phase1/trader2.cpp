#include "receiver.h"

// #include <map>
#include "map.h"
#include "linked_list.h"
#define vector_size 5
std::string notrade = "No trade";
std::vector<int> zerovec(vector_size, 0);
char newline = '\n';
std::string str_newline = "\n";
int g_profit = 0;
//AVLMap b_qoute; // {stock_name, value} best quote which is not cancelled
//AVLMap s_qoute; // {stock_name, value} """

AVLMap stock_index; // from stock to index of it in the array
// AVLMap stocks; // {stock_name, predicted value} 
LinkedList input_lines; // llist of inputted lines


//AVLMap b_best; // {stock_name, predicted value} stores best price of a stock bought ever
//AVLMap s_best; // {stock_name, predicted value} stores best price of a stock bought ever


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

std::vector<int> add2Vectors(const std::vector<int>& v1, const std::vector<int>& v2) {
    // Check if the vectors have the same size
    // if (v1.size() != v2.size()) {
    //     throw std::invalid_argument("Vectors must have the same size");
    // }

    // Create a new vector to store the result
    std::vector<int> result;

    // Iterate through the vectors and add corresponding elements
    for (size_t i = 0; i < v1.size(); ++i) {
        result.push_back(v1[i] + v2[i]);
    }

    return result;
}

std::vector<int> sub2Vectors(const std::vector<int>& v1, const std::vector<int>& v2) {
    // Check if the vectors have the same size
    // if (v1.size() != v2.size()) {
    //     throw std::invalid_argument("Vectors must have the same size");
    // }

    // Create a new vector to store the result
    std::vector<int> result;

    // Iterate through the vectors and add corresponding elements
    for (size_t i = 0; i < v1.size(); ++i) {
        result.push_back(v1[i] - v2[i]);
    }

    return result;
}

// LETS GO 

//O(2^n)

// LL - inputlines - main list of all arrays of input
// n - gonna find powerset of that many elements from start
// arrind - indices that have been considered for sum
// sumvec - the sum of all arrays
// zeroindices - list of all sums that give zero
// pricesum

void f(LinkedList &LL, int n, std::vector<int> &arrind, std::vector<int> &sumvec, LinkedList &zeroindices, int pricesum){
    if ( n==0 ){
        if(compareVectors(sumvec,zerovec)){
            if(pricesum > 0){
                zeroindices.addVector(arrind, pricesum, "a", 'n');
            }
        }
        return;
    }

    arrind.push_back(n-1);
    sumvec = add2Vectors(sumvec, LL.getNodeByIndex(n-1)->data);
    pricesum += LL.getNodeByIndex(n-1)->price;
    f(LL, n-1, arrind, sumvec, zeroindices, pricesum);
    arrind.pop_back();
    sumvec = sub2Vectors(sumvec, LL.getNodeByIndex(n-1)->data);
    pricesum -= LL.getNodeByIndex(n-1)->price;
    f(LL, n-1, arrind, sumvec, zeroindices, pricesum);
    return;
}

// zero indeces will have invalid entries of price, mode and stuffs.
void g(LinkedList &zeroindeces, LinkedList &input_lines){
    //std::cout<<"inside";
    // std::vector<int> profit[zeroindeces.getSize()];
    if(zeroindeces.getSize()==0){
        std::cout<<"No trade\r\n";
        return;
    }
    int maxindex_zeroindeces = 0;
    int maxprofit = zeroindeces.getNodeByIndex(0)->price;
    for(int i=0;i<zeroindeces.getSize();++i){
        if(maxprofit < zeroindeces.getNodeByIndex(i)->price){
            maxprofit = zeroindeces.getNodeByIndex(i)->price;
            maxindex_zeroindeces = i;
        }
    }
    // now maxprofit contains the max profit, and maxindex_zeroindeces contains the indeces..
    // that is the trade we'll be doing, and we delete corresponding trades.
    for(int i = zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data.size()-1; i>=0; --i){ // coming in decending order
        std::string outputting = input_lines.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->inpline;
        std::cout<< outputting.substr(0,outputting.size()-1);
        if(input_lines.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->mode == 'b')std::cout<<"s#\r"<<std::endl;
        else if(input_lines.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->mode == 's')std::cout<<"b#\r"<<std::endl;

        // input_lines.deleteVectorByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i]);
    }
    for(int i = 0; i < zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data.size(); ++i){ // coming in ascending order
        input_lines.deleteVectorByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i]);
    }
    g_profit += maxprofit;    
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
            std::vector<int> append_vector(vector_size,0);
            // for(int i=0;i<vector_size;++i){ //initialize to 0
            //     append_vector[i]=0;
            // }
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
            bool flag1=false; // tells if cancellation happens or not.
            for(int i=0;i < input_lines.getSize(); ++i){
                if(compareVectors(input_lines.getNodeByIndex(i)->data, append_vector)){
                    if(mode != input_lines.getNodeByIndex(i)->mode){
                        input_lines.deleteVectorByIndex(i);
                        flag1=true;
                        break;
                    }
                    else{
                        if(input_lines.getNodeByIndex(i)->price < price){
                            Node* temp = input_lines.getNodeByIndex(i);
                            temp->price = price;
                            temp->inpline = line;
                            temp->mode = mode;
                            flag1=true;
                            break;
                        }
                    }
                }
            }
            if(flag1==false){
                input_lines.addVector(append_vector, price, line, mode);
            }
            else{std::cout<<notrade<<"\r"<<std::endl;continue;}

            // std::cout<<"summa";
            // algo
            LinkedList zeroindices;
            std::vector<int> sumvec(vector_size,0);
            std::vector<int> arrind;
            int n = input_lines.getSize();
            int pricesum = 0;
            f(input_lines, n, arrind, sumvec, zeroindices, pricesum);
            //std::cout<<input_lines.getSize()<<"\n";
            g(zeroindices,input_lines);
            //std::cout<<"after";
            // std::cerr<<input_lines.getSize();
            // //TEMPORARY: checking if f works
            // std::cout<<zeroindices.getSize();
            // for (int i=0;i < zeroindices.getSize(); ++i){
            //     for (const auto& word : zeroindices.getNodeByIndex(i)->data) {
            //         std::cout << "\"" << word << "\"";
            //     }
            //     std::cout<<std::endl;
            // }
            
            

            // ________________________________________
            // out+=  "\n";
            // for(int i=0;i<out.size();i++){
            //     if(out[i]=='\0'){continue;}
            //     std::cout<<out[i];
            // }
            // std::cout<<out<<"\r\n";
        }
        //input_lines.printListInOrder();
        // std::cout<< message << std::endl;



        // ____________ for every message
        if(message[message.size()-1] == '$'){ break;}
    }

// _________________________________________________________
    // std::cout<< out;
    std::cout<<g_profit<<"\r"<<std::endl;
    rcv.terminate();
    return 0;
}
