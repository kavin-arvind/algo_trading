#include<iostream>
#include <fstream>
#include <string>

struct Node {
    std::string broker;
    std::string stock;
    int timestamp;
    int exptime;
    char mode;
    int quantity;
    int price;

    Node(std::string b = "", std::string s = "", int ts = 0, int et = 0, char m = 'n', int q = 0, int p = 0)
        : broker(b), stock(s), timestamp(ts), exptime(et), mode(m), quantity(q), price(p) {}
};

bool valid_for_substr(std::string str, int startPos, int length){
    if (startPos < str.size() && 0<=length && length <= str.size()-startPos) {
        return true;
    } else {
        return false;
    }
}

Node* line_process_to_node(std::string line, bool &valid){
    if(line == "TL"){valid = false; return nullptr;}
    Node* inp = new Node() ;
    valid = true;

    int linear_combo_start = 0;

    //coming from start

    int index = 0;
    int index_prev = 0;
    // timestamp:
    while(line[index] != ' '){
        index++;
    }
    if(valid_for_substr(line,index_prev, index-index_prev)){
        try{inp->timestamp = std::stoi(line.substr(index_prev,index-index_prev));}
        catch (const std::invalid_argument& e){
            inp->timestamp = 0; valid = false; return inp;
        }
    }
    else{valid = false; return inp;}
    index_prev = ++index;

    // broker:
    while(line[index] != ' '){
        index++;
    }

    if(valid_for_substr(line,index_prev, index-index_prev)){
        try{inp->broker = line.substr(index_prev,index-index_prev);}
        catch (const std::invalid_argument& e){
            inp->broker = ""; valid = false; return inp;
        }
    }
    else{valid = false; return inp;}
    index_prev = ++index;

    // b/s:
    while(line[index] != ' '){
        index++;
    }
    if(valid_for_substr(line,index_prev, index-index_prev)){
        if(line.substr(index_prev,index-index_prev) == "SELL"){inp->mode = 's';}
        else if(line.substr(index_prev,index-index_prev) == "BUY"){inp->mode = 'b';}
        else{inp->mode = 'n'; valid = false; return inp;}
    }
    else{valid = false; return inp;}
    index_prev = ++index;

    linear_combo_start = index_prev ; 
    // coming from back for others

    index = line.size()-1;
    index_prev = line.size();
    
    // expairy:
    inp->exptime = inp->timestamp;
    while(line[index] != ' '){
        index--;
    }
    if(valid_for_substr(line,index+1, index_prev - index - 1)){
        try{
            int temp = std::stoi(line.substr(index+1,index_prev - index - 1));
            if(temp == -1){inp->exptime = -1;}
            else{inp->exptime += temp;}
        }
        catch (const std::invalid_argument& e){
            inp->exptime += 0; valid = false; return inp;
        }
    }
    else{valid = false;}
    index_prev = index--;

    // quantity:
    while(line[index] != '#'){
        index--;
    }
    if(valid_for_substr(line,index+1, index_prev - index - 1)){
        try{inp->quantity = std::stoi(line.substr(index+1,index_prev - index - 1));}
        catch (const std::invalid_argument& e){
            inp->quantity = 0; valid = false; return inp;
        }
    }
    else{valid = false; return inp;}
    index--; index_prev = index--;

    if(line[index_prev]!=' '){valid = false; return inp;}

    // price:
    while(line[index] != '$'){
        index--;
    }
    if(valid_for_substr(line,index+1, index_prev - index - 1)){

        try{inp->price = std::stoi(line.substr(index+1,index_prev - index - 1));}
        catch (const std::invalid_argument& e){
            inp->price = 0; valid = false; return inp;
        }
    }
    else{valid = false; return inp;}
    index--; index_prev = index--; 

    if(line[index_prev]!=' '){valid = false; return inp;}
   

    if(valid_for_substr(line,linear_combo_start, index_prev - linear_combo_start)){
        try{inp->stock = line.substr(linear_combo_start,index_prev - linear_combo_start);}
        catch (const std::invalid_argument& e){
            inp->stock = ""; valid = false; return inp;
        }
    }
    else{
        valid = false; return inp;
    }

    return inp;

}