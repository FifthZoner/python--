#include <utility>
#include <unordered_map>

#include "parsing.hpp"
#include "defines.hpp"
#include "checks.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

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

    if (!line.empty() and line[line.length() - 1] != ' '){
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
std::string specialCharacters = ",.(){}[]=+-*/%^!<>:";
std::pair<char, char> doubleOperators[9] =
        {
        std::make_pair<char, char>('=', '='),
        std::make_pair<char, char>('-', '='),
        std::make_pair<char, char>('+', '='),
        std::make_pair<char, char>('*', '='),
        std::make_pair<char, char>('^', '='),
        std::make_pair<char, char>('/', '='),
        std::make_pair<char, char>('!', '='),
        std::make_pair<char, char>('>', '='),
        std::make_pair<char, char>('<', '=')};

// I don't have the motivation to figure out why I can't just throw in std::array instead of vector
std::vector <std::string> doublesToMove = {"+=", "-=" , "*=", "/=", "^="};

std::unordered_map<std::string, uint8_t> tokenMap =
        {
                {"if", ParseStruct::keywordIf},
                {"while", ParseStruct::keywordWhile},
                {"return", ParseStruct::keywordReturn},
                {"implicit", ParseStruct::keywordImplicit},
                {"string", ParseStruct::variableString},
                {"int", ParseStruct::variableInt},
                {"end", ParseStruct::keywordEnd},
                {"else", ParseStruct::keywordElse},
                {"+", ParseStruct::operatorPlus},
                {"-", ParseStruct::operatorMinus},
                {"*", ParseStruct::operatorMultiply},
                {"/", ParseStruct::operatorDivide},
                {"^", ParseStruct::operatorPower},
                {"==", ParseStruct::operatorCompare},
                {">", ParseStruct::operatorCompare},
                {"<", ParseStruct::operatorCompare},
                {"!=", ParseStruct::operatorCompare},
                {">=", ParseStruct::operatorCompare},
                {"<=", ParseStruct::operatorCompare},
                {"=", ParseStruct::operatorAssign}
        };

uint8_t CheckTokenType(const std::string& token){
    if (auto found = tokenMap.find(token); found != tokenMap.end()) {
        return tokenMap[token];
    }
    if (IsVariable(token)){
        return ParseStruct::variableVariable;
    }
    return ParseStruct::variableValue;
}

std::unique_ptr<ParseStruct> ParseLine(std::pair<unsigned int, unsigned int> range, unsigned long long lineNumber){
    #ifdef PYTHON___DEBUG
        std::cout << "Parsing line from function with input:\n";
        for (unsigned int n = range.first; n < range.second; n++){
            std::cout << parsedLine[n] << " ";
        }
        std::cout << "\n";
    #endif
    std::unique_ptr<ParseStruct> parseStruct = nullptr;
    if (range.second - range.first > 0){
        if (IsFunction(parsedLine[range.first])){
            if (functions[parsedLine[range.first]]->returnType() == Variable::none){
                // that means a return-less function line
                return std::make_unique <ParseFunction> (range);
            }
            ParserException("Function with return type as first token!");
            return std::make_unique <ParseStruct> ();
        }
        if (parsedLine[0] == "if"){
            // an if statement
            return std::make_unique <ParseIf> (std::pair <unsigned int, unsigned int> (range.first + 1, range.second), lineNumber);
        }

        if (parsedLine[0] == "while"){
            // a while statement
            return std::make_unique <ParseWhile> (std::pair <unsigned int, unsigned int> (range.first + 1, range.second), lineNumber);
        }
    }
    if (range.second - range.first > 3){
        if ((parsedLine[range.first] == "int" or parsedLine[range.first] == "string" or parsedLine[range.first] == "void")
        and parsedLine[range.first + 2] == "(" and parsedLine[range.second - 1] == ")") {
            return std::make_unique <ParseCustomFunction> (std::pair <unsigned int, unsigned int> (range.first, range.second));
        }
    }
    for (unsigned int n = range.first; n < range.second; n++){
        if (auto found = tokenMap.find(parsedLine[n]); found != tokenMap.end()) {
            switch (tokenMap[parsedLine[n]]){
                case ParseStruct::operatorAssign:
                    parseStruct = std::unique_ptr<ParseStruct>(new ParseAssign(
                            std::pair<unsigned int, unsigned int>(range.first, n),
                            std::pair<unsigned int, unsigned int>(n + 1, range.second)));
                    break;

                default:
                    // this case is a missing function or an error
                    break;
            }
        }
        else {
            // function / variable name or a value
        }
    }
    if (parseStruct == nullptr){
        ParserException("Could not parse anything that makes sense!");
    }

    return parseStruct;
}

std::unique_ptr<ParseStruct> SplitInterpreterLine(std::string line, unsigned long long lineNumber){

    // deleting tabs
    for (long n = 0; n < line.length(); n++){
        if (line[n] == '\t'){
            line.erase(n, 1);
            n--;
        }
    }

    // splitting special characters
    for (unsigned int n = 0; n < line.length(); n++) {
        if (line[n] == '\"') {
            n++;
            while (line[n] != '\"' or (line[n] == '\"' and n > 0 and line[n - 1] == '\\')) {
                n++;
            }
            n++;
        }
        for (char m : specialCharacters){
            if (line[n] == m){
                if (n > 0){
                    if (n > 1){
                        bool isDouble = false;
                        for (auto& k : doubleOperators) {
                            if (k.second == line[n] and k.first == line[n - 1]) {
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
                    for (auto& k : doubleOperators) {
                        if (k.first == line[n] and k.second == line[n + 1]) {
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

    // checking for negative numbers
    for (unsigned int n = 0; n < line.length(); n++){
        if (line[n] == '\"') {
            n++;
            while (line[n] != '\"' or (line[n] == '\"' and n > 0 and line[n - 1] == '\\')) {
                n++;
            }
            n++;
        }
        for (auto m : specialCharacters){
            if (line[n] == m and line.length() > n + 4 and line[n + 2] == '-'){
                line.erase(n + 3, 1);
            }
        }
    }

    // splitting into vector of strings
    parsedLine.clear();
    unsigned int start = 0;
    for (unsigned int n = 0; n < line.length(); n++){
        if (line[n] == '\"'){
            start = n;
            n++;
            while (line[n] != '\"' or (line[n] == '\"' and n > 0 and line[n - 1] == '\\')){
                n++;
            }
            std::string temp = line.substr(start, n - start + 1);
            for (unsigned int m = 1; m < temp.length() - 2; m++){
                if (temp[m] == '\\' and temp[m + 1] == '\"'){
                    temp.erase(m, 1);
                    m--;
                }
            }
            parsedLine.push_back(temp);
            n++;
            start = n;
        }
        if (line[n] == ' '){
            if (n != start) {
                parsedLine.push_back(line.substr(start, n - start));
            }
            start = n + 1;
        }
    }

    if (!line.empty() and line[line.length() - 1] != ' ' and line[line.length() - 1] != '\"'){
        parsedLine.push_back(line.substr(start, line.length() - start));
    }

    // adding changes from double operators to avoid additional ParseStructs
    for (unsigned int n = 0; n < parsedLine.size(); n++){
        for (auto& m : doublesToMove){
            if (parsedLine[n] == m){
                parsedLine.insert(parsedLine.begin() + n + 1, "(");
                parsedLine.insert(parsedLine.end(), 1, ")");
                parsedLine.insert(parsedLine.begin() + n + 1, std::string(1, m[0]));
                parsedLine.insert(parsedLine.begin() + n + 1, parsedLine[n - 1]);
                parsedLine[n] = std::string(1, m[1]);
                break;
            }
        }
    }

    return ParseLine(std::make_pair<unsigned int, unsigned int>(0, parsedLine.size()), lineNumber);
}

