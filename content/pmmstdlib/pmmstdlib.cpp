#include "pmmstdlib.hpp"

void PrepareStandardLibrary(){
    functions["Print"]   = std::make_unique<FunctionBinding> (pmmstd::Print,   Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "message", true)});
    functions["Println"] = std::make_unique<FunctionBinding> (pmmstd::Println, Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "message", true)});
    functions["Input"] =   std::make_unique<FunctionBinding> (pmmstd::Input,   Variable::typeString, std::vector <FunctionVariable> {});
    functions["IsNum"] =   std::make_unique<FunctionBinding> (pmmstd::IsNum,   Variable::typeNum   , std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "token", false )});
    functions["ToNum"] =   std::make_unique<FunctionBinding> (pmmstd::ToNum,   Variable::typeNum, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "value", false )});
    functions["Modulo"] =  std::make_unique<FunctionBinding> (pmmstd::Modulo,  Variable::typeNum, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "value", false ), FunctionVariable(Variable::typeString, "mod", false )});
    functions["Power"] =   std::make_unique<FunctionBinding> (pmmstd::Power,   Variable::typeNum, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "value", false ), FunctionVariable(Variable::typeString, "pow", false )});
}