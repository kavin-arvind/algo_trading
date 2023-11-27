#include "market.h"
#include "header.h"
market::market(int argc, char** argv)
{

}

void market::start()
{
    std::ifstream inputfromoutput("output.txt");
    std::string line;
    bool valid = true;

    while (true)
    {
        // if(!std::getline(inputfromoutput, line)) break;
        std::getline(inputfromoutput, line);
        if(line == "!@") break;

        // now line has the correct line we needed..
        Node* a = line_process_to_node(line,valid);
        if(valid){
        std::cout<<a->broker<<"*"<<a->exptime<<"*"<<a->mode<<"*"<<a->price<<"*"<<a->quantity<<"*"<<a->stock<<"*"<<a->timestamp<<std::endl;
        }
        else{std::cout<<line;}
    }
    
}
