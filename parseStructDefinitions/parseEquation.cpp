#include "../parseStructs.hpp"
#include "../parsing.hpp"
#include "../checks.hpp"
#include "../defines.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::vector <std::string> parsedLine;

ParseEquation::ParseEquation(std::pair<unsigned int, unsigned int> range) {
    #ifdef PYTHON___DEBUG
    std::cout << "Parsing an equation with size of: " << range.second - range.first << "\n";
    #endif

    if ((range.second - range.first) % 2 != 1) {
        ParserException("Equation of wrong size!");
        return;
    }

    // actual parsing and things, brackets will not be supported at first

    // analysing equation, this struct is there for future advanced parsing with brackets
    struct Analyzer {
        std::vector <Analyzer> content;
        uint8_t type;

        explicit Analyzer(uint8_t type) {
            this->type = type;
        }
    };

    std::vector <Analyzer> analyzers;

    for (unsigned int n = range.first; n < range.second; n++) {

        switch (CheckTokenType(parsedLine[n])) {
            case ParseStruct::variableVariable:
                analyzers.emplace_back(ParseStruct::variableVariable);
                break;
            case ParseStruct::variableValue:
                analyzers.emplace_back(ParseStruct::variableValue);
                break;
            case ParseStruct::operatorPlus:
                analyzers.emplace_back(ParseStruct::operatorPlus);
                break;
            case ParseStruct::operatorMinus:
                analyzers.emplace_back(ParseStruct::operatorMinus);
                break;
            default:
                ParserException("Wrong type of token in equation!");
                return;
        }
    }

    ParseStruct* temp = nullptr;
    // parsing the structure, for now simple and only with pluses
    for (int n = analyzers.size() - 2; n >= 0; n -= 2) {
        if (analyzers[n].type == ParseStruct::operatorPlus){
            if (temp != nullptr){
                temp = new ParsePlus(parsedLine[n + range.first - 1], temp);
            }
            else {
                temp = new ParsePlus(parsedLine[n + range.first - 1], parsedLine[n + range.first + 1]);
            }
        }
        else {
            ParserException("Cannot parse equation!");
            return;
        }
    }
    from = std::unique_ptr <ParseStruct> (temp);

}
const uint8_t ParseEquation::type() const{
    return ParseStruct::operatorEquation;
}
std::string ParseEquation::run(uint8_t type) const{
    switch (from->type()){
        case ParseStruct::operatorPlus:
            return reinterpret_cast <ParsePlus*> (from->getPointer())->run(type);
        default:
            InterpreterException("Equation type out of range!");
            return "N/A";
    }
}