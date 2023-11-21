#include <cstdint>

#include "parsing.hpp"

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

    return vec;
}

std::vector <std::vector <std::string>> ParseCommandArguments(const int argc, char** argv) {
    std::vector <std::vector <std::string>> vec;
    for (unsigned int n = 0; n < argc; n++){
        vec.push_back(ParseCommandLine(std::string(argv[n])));
    }
    return vec;
}

std::string specialCharacters = ",.\"(){}[]=+-*/%^!\'<>";

// TODO: add support for multi character things
std::vector <std::string> ParseInterpreterLine(std::string line){
    for (unsigned int n = 0; n < line.length(); n++){
        for (char m : specialCharacters){
            if (line[n] == m){
                if (n > 0){
                    if (line[n - 1] != ' ') {
                        line.insert(n, " ");
                        n++;
                    }
                }
                if (n < line.length() - 1){
                    if (line[n + 1] != ' '){
                        line.insert(n + 1, " ");
                    }
                }

                break;
            }
        }
    }

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

    return vec;
}
