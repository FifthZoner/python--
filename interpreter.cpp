#include <string>

#include "interpreter.hpp"
#include "parsing.hpp"


#include <iostream>
void RunLine(std::string line){
    auto tokens = ParseInterpreterLine(line);
    for (auto& n : tokens){
        std::cout << n << " ";
    }
    std::cout << "\n";
}

void RunInterpreter(){
    RunLine("var=10-(5235+3256)-243");
}