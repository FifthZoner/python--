#include "functionStack.hpp"

extern std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

InstanceLevel::InstanceLevel(uint64_t recallLine) {
    this->recallLine = recallLine;
}

FunctionInstance::FunctionInstance(std::vector<std::string> &arguments, uint64_t startingLine, Function* function) {
    this->currentLine = startingLine;
    // creating arguments
    for (unsigned int n = 0; n < arguments.size(); n++){
        if (function->variables[n].type == Variable::typeInt){
            variables[function->variables[n].name] = std::unique_ptr <Variable> (new VariableInt(std::stoll(arguments[n])));
        }
        else {
            variables[function->variables[n].name] = std::unique_ptr <Variable> (new VariableString(arguments[n]));
        }
    }
}

Variable* NewVariable(std::string& token, long long value){
    if (!functionStack.empty()){
        if (!functionStack.top().variables.empty()){
            // leveled local variable
            functionStack.top().levels.back().variables[token] = std::unique_ptr <Variable> (new VariableInt(value));
            return functionStack.top().levels.back().variables[token].get();
        }
        else {
            // local function-wide variable
            functionStack.top().variables[token] = std::unique_ptr <Variable> (new VariableInt(value));
            return functionStack.top().variables[token].get();
        }
    }
    else if (!globalLevels.empty()){
        // leveled global variable
        globalLevels.back().variables[token] = std::unique_ptr <Variable> (new VariableInt(value));
        return globalLevels.back().variables[token].get();
    }
    else {
        // global variable
        globalVariables[token] = std::unique_ptr <Variable> (new VariableInt(value));
        return globalVariables[token].get();
    }
}

Variable* NewVariable(std::string& token, std::string& value){
}