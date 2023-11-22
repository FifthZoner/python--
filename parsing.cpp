#include <utility>
#include <unordered_map>
#include <cstdint>

#include "parsing.hpp"

std::vector <std::string> ParseCommandLine(const std::string& line) {
    std::vector <std::string> vec;
    unsigned int start = 0;
    for (unsigned int n = 0; n < line.length(); n++) {
        if (line[n] == ' ') {
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

std::string specialCharacters = ",.\"(){}[]=+-*/%^!\'<>:";
std::pair<char, char> doubleOperators[4] =
        {
        std::make_pair<char, char>('=', '='),
        std::make_pair<char, char>('!', '='),
        std::make_pair<char, char>('>', '='),
        std::make_pair<char, char>('<', '=')
                };

std::vector <std::string> SplitInterpreterLine(std::string line){

    // splitting special characters
    for (unsigned int n = 0; n < line.length(); n++) {
        for (char m : specialCharacters){
            if (line[n] == m){
                if (n > 0){
                    if (n > 1){
                        bool isDouble = false;
                        for (auto& m : doubleOperators) {
                            if (m.second == line[n] and m.first == line[n - 1]) {
                                isDouble = true;
                            }
                        }
                        if (!isDouble and line[n - 1] != ' ') {
                            line.insert(n, " ");
                            n++;
                        }
                    }
                    else if (line[n - 1] != ' ') {
                        line.insert(n, " ");
                        n++;
                    }
                }
                if (n < line.length() - 1) {
                    bool isDouble = false;
                    for (auto& m : doubleOperators) {
                        if (m.first == line[n] and m.second == line[n + 1]) {
                            isDouble = true;
                        }
                    }

                    if (!isDouble and line[n + 1] != ' ') {
                        line.insert(n + 1, " ");
                    }
                }

                break;
            }
        }
    }

    // splitting into vector of strings
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

    if (line[line.length() - 1] != ' '){
        vec.push_back(line.substr(start, line.length() - start));
    }

    return vec;
}

// keywordIf, keywordFor, keywordWhile, keywordReturn, keywordDef, keywordImplicit, keywordConvert, variableString, variableInt, variableUnknown, variableValue, operatorPlus, operatorMinus, operatorEqual, operatorAssign, operatorFunction

std::unordered_map<std::string, uint8_t> tokenMap =
        {
                {"if", ParseStruct::keywordIf},
                {"for", ParseStruct::keywordFor},
                {"while", ParseStruct::keywordWhile},
                {"return", ParseStruct::keywordReturn},
                {"string", ParseStruct::keywordString},
                {"int", ParseStruct::keywordInt},
                {"implicit", ParseStruct::keywordImplicit},
                {"convert", ParseStruct::keywordConvert},
                {"=", ParseStruct::operatorAssign}
        };

ParseStruct* ParseLine(std::vector <std::string> line){
    ParseStruct* parseStruct;
    for (auto& n : line){
        //if ( auto findit = myMap.find(id); findit != myMap.end() )
        if (auto found = tokenMap.find(n); found != tokenMap.end()){

        }
    }
}
