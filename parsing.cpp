#include "parsing.hpp"


#include <iostream> //temp
std::vector <std::string> ParseCommandLine(const std::string& line){
    std::vector <std::string> vec;
    unsigned int start = 0;
    for (unsigned int n = 0; n < line.length(); n++){
        if (line[n] == ' '){
            if (n != start) {
                vec.push_back(line.substr(start, n - start));
            }
            start = n + 1;
        }
    }

    if (start != line.length() - 1){
        vec.push_back(line.substr(start, line.length() - start));
    }

    for (auto& n : vec){
        std::cout << n << "\n";
    }

    return vec;
}

std::vector <std::vector <std::string>> ParseCommandArguments(const int argc, char** argv) {
    std::vector <std::vector <std::string>> vec;
    for (unsigned int n = 0; n < argc; n++){
        vec.push_back(ParseCommandLine(std::string(argv[n])));
    }
    return vec;
}