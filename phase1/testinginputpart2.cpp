#include<iostream>
#include<vector>
#include<string>

int main(){

    std::string line = "    asmlop0   1 y%65 -1  100 b   ";
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

    

    for (const auto& word : linelst) {
        std::cout << "\"" << word << "\"" << std::endl;
    }

    return 0;
    
}