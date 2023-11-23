#include<iostream>
#include "map.h"

int main(){
    AVLMap x;
    AVLMap y;
    x.insert("a",1);
    x.insert("b",2);
    y.insert("c",3);
    y.insert("d",4);
    x.remove("a");
    std::cout<<x.containsKey("a")<<std::endl;
    std::cout<<x.containsKey("b")<<std::endl;
    std::cout<<x.containsKey("c")<<std::endl;
    std::cout<<y.getValue("c")<<std::endl;

    y.update("c", 10);
    std::cout<<y.getValue("c")<<std::endl;
    
}