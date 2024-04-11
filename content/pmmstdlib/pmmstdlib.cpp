#include "pmmstdlib.hpp"

void PrepareStandardLibrary(){
    functions["Print"]   = std::make_unique<FunctionBinding> (pmmstd::Print,   Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "message", true)});
    functions["Println"] = std::make_unique<FunctionBinding> (pmmstd::Println, Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "message", true)});
    functions["Input"] =   std::make_unique<FunctionBinding> (pmmstd::Input,   Variable::typeString, std::vector <FunctionVariable> {});
    functions["IsNum"] =   std::make_unique<FunctionBinding> (pmmstd::IsNum,   Variable::typeNum,    std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "token",  false)});
    functions["ToNum"] =   std::make_unique<FunctionBinding> (pmmstd::ToNum,   Variable::typeNum,    std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "value",  false)});
    functions["Modulo"] =  std::make_unique<FunctionBinding> (pmmstd::Modulo,  Variable::typeNum,    std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "value",  false), FunctionVariable(Variable::typeString, "mod", false )});
    functions["Power"] =   std::make_unique<FunctionBinding> (pmmstd::Power,   Variable::typeNum,    std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "value",  false), FunctionVariable(Variable::typeString, "pow", false )});
    functions["System"] =  std::make_unique<FunctionBinding> (pmmstd::System,  Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "command", true)});
    functions["FileReadOpen"] =     std::make_unique<FunctionBinding> (pmmstd::FileReadOpen,    Variable::typeString, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "path",   true)});
    functions["FileReadClose"] =    std::make_unique<FunctionBinding> (pmmstd::FileReadClose,   Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "handle", true)});
    functions["FileReadIsOpen"] =   std::make_unique<FunctionBinding> (pmmstd::FileReadIsOpen,  Variable::typeNum,    std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "handle", true)});
    functions["FileReadLine"] =     std::make_unique<FunctionBinding> (pmmstd::FileReadLine,    Variable::typeString, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "handle", true)});
    functions["FileReadString"] =   std::make_unique<FunctionBinding> (pmmstd::FileReadString,  Variable::typeString, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "handle", true)});
    functions["FileReadNum"] =      std::make_unique<FunctionBinding> (pmmstd::FileReadNum,     Variable::typeNum,    std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "handle", true)});
    functions["FileWriteOpen"] =    std::make_unique<FunctionBinding> (pmmstd::FileWriteOpen,   Variable::typeString, std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "path",   true)});
    functions["FileWriteClose"] =   std::make_unique<FunctionBinding> (pmmstd::FileWriteClose,  Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "handle", true)});
    functions["FileWriteIsOpen"] =  std::make_unique<FunctionBinding> (pmmstd::FileWriteIsOpen, Variable::typeNum,    std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "handle", true)});
    functions["FileWriteLine"] =    std::make_unique<FunctionBinding> (pmmstd::FileWriteLine,   Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "handle", true), FunctionVariable(Variable::typeString, "value", true)});
    functions["FileWrite"] =        std::make_unique<FunctionBinding> (pmmstd::FileWrite,       Variable::none,       std::vector <FunctionVariable> {FunctionVariable(Variable::typeString, "handle", true), FunctionVariable(Variable::typeString, "value", true)});
}