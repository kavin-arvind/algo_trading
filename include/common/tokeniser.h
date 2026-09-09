#pragma once

#include <iostream>
#include <string.h>

class Tokeniser {
    private:
        inline static int k = 0;
    public:
        static std::string tokeniser(const std::string& txt, char l) {
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
            if(txt[k-1]=='\0'){return "";}
            return temp;
        }
        inline static void tokeniserResetTo(int pos = 0) {
            k = pos;
        }
        inline static int tokeniserPosition() {
            return k;
        }
};