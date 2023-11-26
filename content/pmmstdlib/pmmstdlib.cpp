#include "pmmstdlib.hpp"

std::string returnValueString;

void PrepareStandardLibrary(){
    functions["Print"]   = std::make_unique<FunctionBinding> (pmmstd::Print,   Variable::none, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "message", true)});
    functions["Println"] = std::make_unique<FunctionBinding> (pmmstd::Println, Variable::none, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "message", true)});
}