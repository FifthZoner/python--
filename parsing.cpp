#include <utility>
#include <unordered_map>
#include <cmath>

#include "parsing.hpp"
#include "content/defines.hpp"
#include "interpretation/checks.hpp"
#include "interpretation/runtime.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#include <cmath>

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
std::string specialCharacters = ",(){}[]=+-*/%^!<>:";
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
                {"num", ParseStruct::variableNum},
                {"end", ParseStruct::keywordEnd},
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
            ParserException(returnValueString = "Function with return type as first token!");
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

        if (parsedLine[0] == "return"){
            // a return statement
            return std::make_unique <ParseReturn> (std::pair <unsigned int, unsigned int> (range.first, range.second));
        }
    }
    if (range.second - range.first > 3){
        if ((parsedLine[range.first] == "num" or parsedLine[range.first] == "string" or parsedLine[range.first] == "void")
        and parsedLine[range.first + 2] == "(" and parsedLine[range.second - 1] == ")") {
            return std::make_unique <ParseCustomFunction> (std::pair <unsigned int, unsigned int> (range.first, range.second));
        }
    }
    if (range.second - range.first >= 5 and IsArray(parsedLine[range.first]) and parsedLine[range.first + 1] == ":"
        and parsedLine[range.first + 3] == "(" and parsedLine[range.second - 1] == ")") {
        // this is the case with array functions
        Variable* arr = GetVariable(parsedLine[range.first]);
        if (parsedLine[range.first + 2] == "clear") {
            if (arr->type() == Variable::typeNum) {
                reinterpret_cast<VariableNum*>(arr)->values.clear();
                return nullptr;
            }
            else {
                reinterpret_cast<VariableString*>(arr)->values.clear();
                return nullptr;
            }
        }
        else if (parsedLine[range.first + 2] == "push") {
            std::unique_ptr <ParseStruct> temp =
                    std::unique_ptr <ParseStruct> (ParseMathematicalOperation(
                            std::pair<unsigned int, unsigned int> (range.first + 4, range.second - 1)));
            std::string valueString = RunValueReturning(temp.get(), Variable::typeNum);
            if (arr->type() == Variable::typeNum) {
                if (not IsConvertibleToNum(valueString)) {
                    ParserException("Cannot assign a string to a numeric array!");
                }
                reinterpret_cast<VariableNum*>(arr)->values.emplace_back(std::stold(valueString));
                return nullptr;
            }
            else {
                reinterpret_cast<VariableString*>(arr)->values.emplace_back(valueString);
                return nullptr;
            }
        }
        else if (parsedLine[range.first + 2] == "resize") {
            std::unique_ptr <ParseStruct> temp =
                    std::unique_ptr <ParseStruct> (ParseMathematicalOperation(
                            std::pair<unsigned int, unsigned int> (range.first + 4, range.second - 1)));
            std::string valueString = RunValueReturning(temp.get(), Variable::typeNum);
            if (not IsConvertibleToNum(valueString)) {
                ParserException("Array index number cannot be a string!");
            }
            long long size = std::floor(std::stold(valueString));
            long long arraySize = 0;
            if (arr->type() == Variable::typeNum) {
                arraySize = reinterpret_cast<VariableNum*>(arr)->values.size();
            }
            else {
                arraySize = reinterpret_cast<VariableString*>(arr)->values.size();
            }
            if (size  < 0) {
                ParserException("Given size is negative!");
            }
            if (arr->type() == Variable::typeNum) {
                if (size > arraySize) {
                    for (unsigned long long n = arraySize; n < size; n++) {
                        reinterpret_cast<VariableNum*>(arr)->values.emplace_back(0);
                    }
                }
                else {
                    for (long long n = arraySize; n != size; n--) {
                        reinterpret_cast<VariableNum*>(arr)->values.pop_back();
                    }
                }

                return nullptr;
            }
            else {
                if (size > arraySize) {
                    for (unsigned long long n = arraySize; n < size; n++) {
                        reinterpret_cast<VariableString*>(arr)->values.emplace_back("0");
                    }
                }
                else {
                    for (long long n = arraySize; n != size; n--) {
                        reinterpret_cast<VariableString*>(arr)->values.pop_back();
                    }
                }
                return nullptr;
            }
        }
        else if (parsedLine[range.first + 2] == "pop") {
            if (arr->type() == Variable::typeNum) {
                if (reinterpret_cast<VariableNum*>(arr)->values.empty()) {
                    ParserException("Cannot pop an empty array!");
                }
                reinterpret_cast<VariableNum*>(arr)->values.pop_back();
                return nullptr;
            }
            else {
                if (reinterpret_cast<VariableString*>(arr)->values.empty()) {
                    ParserException("Cannot pop an empty array!");
                }
                reinterpret_cast<VariableString*>(arr)->values.pop_back();
                return nullptr;
            }
        }
        ParserException("Invalid array method!");

    }
    // definition of a new array
    if (range.second - range.first >= 6){
        if ((parsedLine[range.first] == "num" or parsedLine[range.first] == "string")
            and parsedLine[range.first + 2] == "[" and parsedLine[range.first + 3] == "]"
            and parsedLine[range.first + 4] == "=") {
            return std::make_unique <ParseAssign> (std::pair <unsigned int, unsigned int> (range.first, range.first + 4)
                    ,std::pair <unsigned int, unsigned int> (range.first + 5, range.second),  true);
        }
    }
    if (range.second - range.first >= 3){
        if (IsArray(parsedLine[range.first])
            and parsedLine[range.first + 1] == "=") {
            return std::make_unique <ParseAssign> (std::pair <unsigned int, unsigned int> (range.first, range.first + 3)
                    ,std::pair <unsigned int, unsigned int> (range.first + 3, range.second),  true);
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

void SplitInterpreterLine(std::string line, unsigned long long lineNumber){

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
                for (long long k = n - 1; k >= 0; k--) {
                    parsedLine.insert(parsedLine.begin() + n + 1, parsedLine[k]);
                }
                parsedLine[n] = std::string(1, m[1]);
                break;
            }
        }
    }

    return;
}

