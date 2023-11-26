#include "../parseStructs.hpp"
#include "../parsing.hpp"
#include "../checks.hpp"
#include "../defines.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::vector <std::string> parsedLine;

ParseStruct* ParseMathematicalOperation(std::pair <unsigned int, unsigned int> range){

    std::cout << "Math: ";
    for (unsigned int n = range.first; n < range.second; n++) {
        std::cout << parsedLine[n] << " ";
    }
    std::cout << "\n";

    // variable / value handling
    if (range.second - range.first == 1){
        if (IsGlobalVariable(parsedLine[range.first]) or IsLocalVariable(parsedLine[range.first])) {
            return new ParseVariable(parsedLine[range.first]);
        }
        else {
            // value
            return new ParseValue(parsedLine[range.first]);
        }
    }

    // checking for brackets

    // this is the case when brackets take the whole equation;
    if (parsedLine[range.first] == "(" and parsedLine[range.second - 1] == ")"){
        return ParseMathematicalOperation(std::pair <unsigned int, unsigned int> (range.first + 1, range.second - 1));
    }

    unsigned int bracketLevel = 0;
    for (unsigned int n = range.first; n < range.second; n++) {
        if (parsedLine[n] == "("){
            bracketLevel++;
        }
        else if (parsedLine[n] == ")") {
            if (bracketLevel == 0){
                ParserException("Mathematical operation invalid due to brackets!");
                return new ParseStruct();
            }
            bracketLevel--;
        }
    }
    if (bracketLevel != 0) {
        ParserException("Mathematical operation invalid due to brackets!");
        return new ParseStruct();
    }

    // checking for operators from the last in order to first that is:
    // + -> - -> * -> / -> ^ -> (), - must be second to allow for this specific implementation

    std::pair <long, long> current(range.first, range.second);

    // example: 1 + 5 - 6 + 5 * 5 / 6 + 3
    // -> (1 + 5 - 6 + 5 * 5 / 6) + (3)
    // -> ((1 + 5 - 6) + (5 * 5 / 6)) + (3)
    // -> (((1) + (5 - 6)) + (5 * 5 / 6)) + (3)
    for (unsigned int n = current.second - 1; n >= current.first; n--){
        if (parsedLine[n] == "+"){
            return new ParsePlus(std::pair <unsigned int, unsigned int> (current.first, n), std::pair <unsigned int, unsigned int> (n + 1, current.second));
        }
    }

    // (((1) + (5 - 6)) + (5 * 5 / 6)) + (3)
    // -> (((1) + ((5) - (6))) + (5 * 5 / 6)) + (3)
    for (unsigned int n = current.second - 1; n >= current.first; n--){
        if (parsedLine[n] == "-"){
            return new ParseMinus(std::pair <unsigned int, unsigned int> (current.first, n), std::pair <unsigned int, unsigned int> (n + 1, current.second));
        }
    }

    // (((1) + ((5) - (6))) + (5 * 5 / 6)) + (3)
    // -> (((1) + ((5) - (6))) + ((5) * (5 / 6))) + (3)
    for (unsigned int n = current.second - 1; n >= current.first; n--){
        if (parsedLine[n] == "*"){
            return new ParseMultiply(std::pair <unsigned int, unsigned int> (current.first, n), std::pair <unsigned int, unsigned int> (n + 1, current.second));
        }
    }

    // (((1) + ((5) - (6))) + ((5) * (5 / 6))) + (3)
    // -> (((1) + ((5) - (6))) + ((5) * ((5) / (6)))) + (3)
    for (unsigned int n = current.second - 1; n >= current.first; n--){
        if (parsedLine[n] == "/"){
            return new ParseDivide(std::pair <unsigned int, unsigned int> (current.first, n), std::pair <unsigned int, unsigned int> (n + 1, current.second));
        }
    }

    for (unsigned int n = current.second - 1; n >= current.first; n--){
        if (parsedLine[n] == "^"){
            return new ParsePower(std::pair <unsigned int, unsigned int> (current.first, n), std::pair <unsigned int, unsigned int> (n + 1, current.second));
        }
    }

    // brackets are handled last, so they will be executed via the first and last check at the beginning

    // this should never get here
    ParserException("Parsing of mathematical operation failed!");
    return new ParseStruct();
}