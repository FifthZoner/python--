#include <unordered_map>

#include "../parseStructs.hpp"
#include "../checks.hpp"
#include "../defines.hpp"
#include "../parsing.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::vector <std::string> parsedLine;
extern std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;


ParseAssign::ParseAssign(std::pair<unsigned int, unsigned int> left, std::pair<unsigned int, unsigned int> right){
    #ifdef PYTHON___DEBUG
    std::cout << "Parsing assign operator with left input:\n";
    for (unsigned int n = left.first; n < left.second; n++){
        std::cout << parsedLine[n] << " ";
    }
    std::cout << "\nRight side input:\n";
    for (unsigned int n = right.first; n < right.second; n++){
        std::cout << parsedLine[n] << " ";
    }
    std::cout << "\n";
    #endif

    // left side parsing
    if (parsedLine[left.first] == "implicit"){
        #ifdef PYTHON___DEBUG
        std::cout << "Added implicit to left\n";
        #endif
        target = std::unique_ptr<ParseStruct>(new ParseImplicit(std::pair<unsigned int, unsigned int>(left.first + 1, left.second)));
    }
    else if (left.second - left.first > 2){
        ParserException("Left side is wrong!");
        return;
    }
    if (left.second - left.first == 2){
        if (parsedLine[left.first] == "string"){
            #ifdef PYTHON___DEBUG
            std::cout << "Added string to left\n";
            #endif
            target = std::unique_ptr<ParseStruct>(new ParseString(parsedLine[left.first + 1]));
        }
        else if (parsedLine[left.first] == "int"){
            #ifdef PYTHON___DEBUG
            std::cout << "Added int to left\n";
            #endif
            target = std::unique_ptr<ParseStruct>(new ParseInt(parsedLine[left.first + 1]));
        }
    }
    else if (left.second - left.first == 1){
        // existing variable check
        if (IsGlobalVariable(parsedLine[left.first])){
            target = std::unique_ptr<ParseStruct>(new ParseVariable(parsedLine[left.first]));
        }
        else {
            ParserException("Left side is not a variable!");
            return;
        }
    }
    else {
        ParserException("Error that I forgot what it means!");
        return;
    }


    if (right.second - left.first == 0){
        ParserException("No values on right side!");
        return;
    }

    if (right.second - right.first == 1){
        // one operator on right
        if (IsGlobalVariable(parsedLine[right.first])){
        #ifdef PYTHON___DEBUG
            std::cout << "Added global variable to right\n";
        #endif
            from = std::unique_ptr<ParseStruct>(new ParseVariable(parsedLine[right.first]));
        }
        else {
        #ifdef PYTHON___DEBUG
            std::cout << "Added value to right\n";
        #endif
            from = std::unique_ptr<ParseStruct>(new ParseValue(parsedLine[right.first]));
        }
    }
    else {
        from = std::unique_ptr<ParseStruct>(new ParseEquation(right));
    }
}
const uint8_t ParseAssign::type() const {
    return ParseStruct::operatorAssign;
}
void ParseAssign::run(){

    Variable* var = nullptr;

    switch (target->type()){
        case ParseStruct::variableVariable:
            var = reinterpret_cast <ParseVariable*>(target->getPointer())->run();
            break;
        case ParseStruct::variableInt:
            var = reinterpret_cast <ParseInt*>(target->getPointer())->run();
            break;

        default:
            InterpreterException("Wrong assignment target!");
            return;
    }

    #ifdef PYTHON___DEBUG
    std::cout << "\" was assigned value ";
    #endif

    switch (from->type()){
        case ParseStruct::variableValue:

            switch (var->type()){
                case Variable::typeInt:

                    reinterpret_cast <VariableInt*>(var->getPointer())->value = std::stoll(reinterpret_cast <ParseValue*>(from->getPointer())->run());

                    break;

                case Variable::typeString:

                    reinterpret_cast <VariableString*>(var->getPointer())->value = reinterpret_cast <ParseValue*>(from->getPointer())->run();

                    break;

            }

            break;

        case ParseStruct::variableVariable:

            switch (var->type()){
                case Variable::typeInt:

                    reinterpret_cast <VariableInt*>(var->getPointer())->value = reinterpret_cast <VariableInt*> (reinterpret_cast <ParseVariable*> (from->getPointer())->run())->value;

                    break;

                case Variable::typeString:

                    reinterpret_cast <VariableString*>(var->getPointer())->value = reinterpret_cast <VariableString*> (reinterpret_cast <ParseVariable*> (from->getPointer())->run())->value;

                    break;

            }

            break;

        case ParseStruct::operatorEquation:

            switch (var->type()){
                case Variable::typeInt:

                    reinterpret_cast <VariableInt*>(var->getPointer())->value = std::stoll(reinterpret_cast <ParseEquation*> (from->getPointer())->run(ParseStruct::variableInt));

                    break;

                case Variable::typeString:

                    reinterpret_cast <VariableString*>(var->getPointer())->value = reinterpret_cast <ParseEquation*> (from->getPointer())->run(ParseStruct::variableString);

                    break;

            }

            break;

        default:
            InterpreterException("Wrong assignment origin!");
            break;
    }

    #ifdef PYTHON___DEBUG
    std::cout  << "\"\n";
    #endif

}

ParsePlus::ParsePlus(const std::string& left, const std::string& right) {
    switch (CheckTokenType(left)){
        case ParseStruct::variableVariable:
            this->left = std::unique_ptr <ParseStruct> (new ParseVariable(left));
            break;
        case ParseStruct::variableValue:
            this->left = std::unique_ptr <ParseStruct> (new ParseValue(left));
            break;
        default:
            ParserException("Wrong values in left side plus parsing!");
            return;
    }
    switch (CheckTokenType(right)){
        case ParseStruct::variableVariable:
            this->right = std::unique_ptr <ParseStruct> (new ParseVariable(right));
            break;
        case ParseStruct::variableValue:
            this->right = std::unique_ptr <ParseStruct> (new ParseValue(right));
            break;
        default:
            ParserException("Wrong values in right size plus parsing!");
            return;
    }

}
ParsePlus::ParsePlus(const std::string& left, std::unique_ptr <ParseStruct>& right){
    switch (CheckTokenType(left)){
        case ParseStruct::variableVariable:
            this->left = std::unique_ptr <ParseStruct> (new ParseVariable(left));
            break;
        case ParseStruct::variableValue:
            this->left = std::unique_ptr <ParseStruct> (new ParseValue(left));
            break;
        default:
            ParserException("Wrong values in left side plus parsing!");
            return;
    }
    //this->right = std::move(right);
    // but WHY DOES IT NOT WORK
    this->right = std::unique_ptr <ParseStruct> (new ParseValue(reinterpret_cast<ParsePlus*>(right->getPointer())->run(ParseStruct::variableInt)));
}
[[nodiscard]] const uint8_t ParsePlus::type() const {
    return ParseStruct::operatorPlus;
}
std::string ParsePlus::run(uint8_t type){

    if (type == ParseStruct::variableInt){

        long long value = 0;
        if (left->type() == ParseStruct::variableVariable){
            value = reinterpret_cast <VariableInt*> (reinterpret_cast <ParseVariable*> (left->getPointer())->run())->value;
        }
        else if (left->type() == ParseStruct::variableValue){
            value = std::stoll(reinterpret_cast <ParseValue*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPlus){
            value = std::stoll(reinterpret_cast <ParsePlus*> (left->getPointer())->run(type));
        }
        if (right->type() == ParseStruct::variableVariable){
            return std::to_string(value + reinterpret_cast <VariableInt*> (reinterpret_cast <ParseVariable*> (right->getPointer())->run())->value);
        }
        else if (right->type() == ParseStruct::variableValue){
            return std::to_string(value + std::stoll(reinterpret_cast <ParseValue*> (right->getPointer())->run()));
        }
        else if (right->type() == ParseStruct::operatorPlus){
            return std::to_string(value + std::stoll(reinterpret_cast <ParsePlus*> (left->getPointer())->run(type)));
        }
    }
    else if (type == ParseStruct::variableString) {
        std::string value = "";
        if (left->type() == ParseStruct::variableVariable){
            value = reinterpret_cast <VariableString*> (reinterpret_cast <ParseVariable*> (left->getPointer())->run())->value;
        }
        else if (left->type() == ParseStruct::variableValue){
            value = reinterpret_cast <ParseValue*> (left->getPointer())->run();
        }
        else if (left->type() == ParseStruct::operatorPlus){
            value = reinterpret_cast <ParsePlus*> (left->getPointer())->run(type);
        }
        if (right->type() == ParseStruct::variableVariable){
            return value + reinterpret_cast <VariableString*> (reinterpret_cast <ParseVariable*> (right->getPointer())->run())->value;
        }
        else if (right->type() == ParseStruct::variableValue){
            return value + reinterpret_cast <ParseValue*> (right->getPointer())->run();
        }
        else if (right->type() == ParseStruct::operatorPlus){
            return value + reinterpret_cast <ParsePlus*> (left->getPointer())->run(type);
        }
    }
    else {
        InterpreterException("Wrong variable type in plus!");
    }
    return "";
}