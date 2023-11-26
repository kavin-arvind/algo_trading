#include "receiver.h"

#include "header.h"
std::string notrade = "No trade";
char newline = '\n';
std::string str_newline = "\n";
int g_profit3 = 0;
//AVLMap b_qoute; // {stock_name, value} best quote which is not cancelled
//AVLMap s_qoute; // {stock_name, value} """

AVLMap stock_index3; // from stock to index of it in the array
// AVLMap stocks; // {stock_name, predicted value} 
LinkedList3 input_lines3; // llist of inputted lines
std::vector<int> zerovec3(stock_index3.getSize(), 0);

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

std::string string_converter(std::string input, int num){
    int index_number = input.size();
    int index_number_end = input.size();
    int space_counter = 0;
    for(int i= input.size()-1 ; i>=0; --i){
        if(input[i]==' '){space_counter++;}
        if(space_counter>=2){break;}
        if(space_counter==1){index_number--;}
        if(space_counter==0){index_number--;index_number_end--;}
    }
    return input.substr(0,index_number)+std::to_string(num)+input.substr(index_number_end-1,input.size());
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
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vectors must have the same size");
    }

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
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vectors must have the same size");
    }

    // Create a new vector to store the result
    std::vector<int> result;

    // Iterate through the vectors and add corresponding elements
    for (size_t i = 0; i < v1.size(); ++i) {
        result.push_back(v1[i] - v2[i]);
    }

    return result;
}

void ixVector(std::vector<int>& vec, int multiplier) {
    for (auto& element : vec) {
        element *= multiplier;
    }
}

// LETS GO 

//O(2^n)

// LL - inputlines - main list of all arrays of input
// n - gonna find powerset of that many elements from start
// arrind - indices that have been considered for sum
// sumvec - the sum of all arrays
// zeroindices - list of all sums that give zero
// pricesum

void generateArrays(const std::vector<int>& original, std::vector<int>& current, int index, std::vector<int> &arrind, std::vector<int> &sumvec, LinkedList3 &zeroindices, LinkedList3 &zeroquants, std::vector<int> &arrprices, std::vector<std::vector<int>*> &arrvec) {
    // Base case: print the current array
    if (index == original.size()) {
        for (int i=0;i < current.size(); ++i){
            std::vector<int> arrvec_temp = *arrvec[i]; 
            ixVector(arrvec_temp,current[i]); 
            sumvec = add2Vectors(sumvec, arrvec_temp);
        }
        if (compareVectors(sumvec,zerovec3)){
            int pricesum=0;
            for (int i=0;i < current.size(); ++i){
                pricesum = pricesum + arrprices[i]*current[i];
            }
            if (pricesum > 0){
                // std::cout<<"got an arbit"<<std::endl;
                zeroindices.addVector(arrind, pricesum, "a", 'n', 1);
                zeroquants.addVector(current, pricesum, "a", 'n', 1);
            }
        }
        else{ixVector(sumvec,0);}
        return;
    }

    // Recursively generate arrays for the current index
    for (int i = 0; i <= original[index]; ++i) {
        current[index] = i;
        //generateArrays(original, current, index + 1);
        generateArrays(original, current, index + 1, arrind, sumvec, zeroindices, zeroquants, arrprices, arrvec);
    }
}


void f3(LinkedList3 &LL, int n, std::vector<int> &arrind, std::vector<int> &sumvec, LinkedList3 &zeroindices, LinkedList3 &zeroquants, std::vector<int> &arrprices, std::vector<int> &arrquant, std::vector<std::vector<int>*> &arrvec){
    if ( n==0 ){
        int siz = arrind.size();
        std::vector<int> current(arrquant.size(),0);
        generateArrays(arrquant, current, 0, arrind, sumvec, zeroindices, zeroquants, arrprices, arrvec);
        return;
    }


    Node3* tempo = LL.getNodeByIndex(n-1);
    arrind.push_back(n-1);
    //sumvec = add2Vectors(sumvec, LL.getNodeByIndex(n-1)->data);
    arrquant.push_back(tempo->quantity);
    std::vector<int>* tempo_data = &tempo->data;
    arrvec.push_back(tempo_data); 
    arrprices.push_back(tempo->price);
    //pricesum += LL.getNodeByIndex(n-1)->price;
    f3(LL, n-1, arrind, sumvec, zeroindices, zeroquants, arrprices, arrquant, arrvec);
    arrind.pop_back();
    //sumvec = sub2Vectors(sumvec, LL.getNodeByIndex(n-1)->data);
    arrquant.pop_back(); 
    arrvec.pop_back(); 
    arrprices.pop_back(); 
    //pricesum -= LL.getNodeByIndex(n-1)->price;
    f3(LL, n-1, arrind, sumvec, zeroindices, zeroquants, arrprices, arrquant, arrvec);
    return;
}

// zero indeces will have invalid entries of price, mode and stuffs.
void g3(LinkedList3 &zeroindeces, LinkedList3 &zeroquants, LinkedList3 &input_lines3){
    //std::cout<<"inside";
    // std::vector<int> profit[zeroindeces.getSize()];
    if(zeroindeces.getSize()==0){
        std::cout<<notrade<<"\r"<<std::endl;
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
        std::string outputting = input_lines3.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->inpline; // contains the whole line including b and quantity
        outputting = string_converter(outputting, zeroquants.getNodeByIndex(maxindex_zeroindeces)->data[i]);
        std::cout<< outputting.substr(0,outputting.size()-1);
        if(input_lines3.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->mode == 'b')std::cout<<"s#\r"<<std::endl;
        else if(input_lines3.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->mode == 's')std::cout<<"b#\r"<<std::endl;
    }// printing is over. Now, deletion/updation reqd

    for(int i = 0; i < zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data.size(); ++i){ // coming in ascending order
        if(input_lines3.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->quantity <= zeroquants.getNodeByIndex(maxindex_zeroindeces)->data[i]){
            input_lines3.deleteVectorByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i]);
        }
        else{ // something is remaining
            input_lines3.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->quantity -= zeroquants.getNodeByIndex(maxindex_zeroindeces)->data[i];
            input_lines3.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->inpline = string_converter(input_lines3.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->inpline , input_lines3.getNodeByIndex(zeroindeces.getNodeByIndex(maxindex_zeroindeces)->data[i])->quantity);
        }
    }
    g_profit3 += maxprofit;    
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

            // Quantity is 2rd last element
            std::string quant_str = linelst[linelst.size() - 2];
            int quant;
            quant = stoi(quant_str);
            try{
                quant = stoi(quant_str);
            }
            catch (const std::invalid_argument& e){
                std::cerr << "Invalid argument quant: " << quant_str << std::endl;
                quant = 0;
            }

            // price is 3rd last element
            std::string price_str = linelst[linelst.size() - 3];
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
            linelst.pop_back(); // popping quant
            linelst.pop_back(); // popping price

            // process through the input vector
            std::vector<int> append_vector(stock_index3.getSize(),0);
            // for(int i=0;i<stock_index3.getSize();++i){ //initialize to 0
            //     append_vector[i]=0;
            // }
            for (int i=0; i < linelst.size(); i=i+2){
                if(mode=='b'){// no problem.. we just append to the list
                    if(stock_index3.containsKey(linelst[i])){
                        append_vector[stock_index3.getValue(linelst[i])] = stoi(linelst[i+1]);
                    }
                    else{
                        int sz = stock_index3.getSize();
                        stock_index3.insert(linelst[i],sz);
                        //append_vector[sz] = stoi(linelst[i+1]);
                        append_vector.push_back(stoi(linelst[i+1]));
                        for(int j=0;j<input_lines3.getSize();j++){
                            input_lines3.getNodeByIndex(j)->data.push_back(0);
                        }
                        zerovec3.push_back(0);
                    }
                }
                else if(mode=='s'){// take negative of everything
                    if(stock_index3.containsKey(linelst[i])){
                        append_vector[stock_index3.getValue(linelst[i])] = (-1)*stoi(linelst[i+1]);
                    }
                    else{
                        int sz = stock_index3.getSize();
                        stock_index3.insert(linelst[i],sz);
                        //append_vector[sz] = (-1)*stoi(linelst[i+1]);
                        append_vector.push_back((-1)*stoi(linelst[i+1]));
                        for(int j=0;j<input_lines3.getSize();j++){
                            input_lines3.getNodeByIndex(j)->data.push_back(0);
                        }
                        zerovec3.push_back(0);
                    }
                }
            }

            //CANCELLATION LAWS
            int flag2=0; // tells if cancellation happens or not.
            for(int i=0;i < input_lines3.getSize(); ++i){
                if(compareVectors(input_lines3.getNodeByIndex(i)->data, append_vector)){
                    if(mode != input_lines3.getNodeByIndex(i)->mode && price == input_lines3.getNodeByIndex(i)->price){
                        if(quant < input_lines3.getNodeByIndex(i)->quantity){
                            Node3* temp = input_lines3.getNodeByIndex(i);
                            temp->quantity = input_lines3.getNodeByIndex(i)->quantity - quant;
                            //INPUT LINE HAS TO CHANGE HERE WRITE THAT CODE BELOW
                            temp->inpline = string_converter(temp->inpline,temp->quantity);
                            //------------------->
                            flag2=1;
                            break;

                        }
                        else if(quant == input_lines3.getNodeByIndex(i)->quantity){ // same quant means total cancellation
                            input_lines3.deleteVectorByIndex(i);
                            flag2=1;
                            break;
                        }
                        else{// quant > input_lines3.getNodeByIndex(i)->quantity
                            quant = quant - input_lines3.getNodeByIndex(i)->quantity;
                            input_lines3.deleteVectorByIndex(i);
                            //LINE PROCESSING HAS TO BE DONE
                            line = string_converter(line,quant);
                            flag2=0;
                            break;
                        }
                    }
                    else if(mode == input_lines3.getNodeByIndex(i)->mode && price == input_lines3.getNodeByIndex(i)->price){
                        Node3* temp = input_lines3.getNodeByIndex(i);
                        quant = quant + temp->quantity;
                        input_lines3.deleteVectorByIndex(i);
                        //LINE PROCESSING HAS TO BE DONE
                        line = string_converter(line,quant);
                        flag2=0;
                        break;
                    }
                }
            }
            if(flag2==0){
                input_lines3.addVector(append_vector, price, line, mode, quant);
                // input_lines3.printListInOrder();
            }
            else if(flag2==1){std::cout<<notrade<<"\r"<<std::endl;continue;}

            // std::cout<<"summa";
            // algo
            LinkedList3 zeroindices;
            LinkedList3 zeroquants;
            std::vector<int> sumvec(stock_index3.getSize(),0);
            std::vector<int> arrind;
            std::vector<int> arrquant;
            std::vector<int> arrprices;
            std::vector<std::vector<int>*> arrvec;
            int n = input_lines3.getSize();
            int pricesum = 0;
            f3(input_lines3, n, arrind, sumvec, zeroindices, zeroquants, arrprices, arrquant, arrvec);
            //std::cout<<input_lines3.getSize()<<"\n";
            // std::cout<<std::endl;zeroindices.printListInOrder();std::cout<<std::endl;
        
            g3(zeroindices, zeroquants, input_lines3);
                        
            // ________________________________________
        }
        //input_lines3.printListInOrder();
        // std::cout<< message << std::endl;

        // ____________ for every message
        if(message[message.size()-1] == '$'){ break;}
    }

// _________________________________________________________
    // std::cout<< out;
    std::cout<<g_profit3<<"\r"<<std::endl;
    rcv.terminate();
    return 0;
}
