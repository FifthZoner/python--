#include "pmmstdlib.hpp"

void PrepareStandardLibrary(){
    functions["Print"]   = std::make_unique<FunctionBinding> (pmmstd::Print,   Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "message", true)});
    functions["Println"] = std::make_unique<FunctionBinding> (pmmstd::Println, Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "message", true)});
    functions["Input"] =   std::make_unique<FunctionBinding> (pmmstd::Input,   Variable::typeString, std::vector <FunctionVariable> {});
    functions["IsInt"] =   std::make_unique<FunctionBinding> (pmmstd::IsInt,   Variable::typeInt   , std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "token", false )});
    functions["ToInt"] =   std::make_unique<FunctionBinding> (pmmstd::ToInt,   Variable::typeInt, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "value", false )});
    functions["Modulo"] =  std::make_unique<FunctionBinding> (pmmstd::Modulo,  Variable::typeInt, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "value", false ), FunctionVariable(Variable::typeString, "mod", false )});
    functions["Power"] =   std::make_unique<FunctionBinding> (pmmstd::Power,   Variable::typeInt, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "value", false ), FunctionVariable(Variable::typeString, "pow", false )});
}