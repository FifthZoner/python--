#include <iostream>

#include "pmmstdlib.hpp"
#include "../../exceptions.hpp"

// c++ standard library bindings for printing functions
namespace pmmstd {
    void Print(std::vector <Variable*>& arguments){
        if (arguments.size() != 1){
            InterpreterException("Wrong argument count in \"void Print(implicit string message)\"!");
            return;
        }
        if (arguments[0]->type() != Variable::typeString){
            InterpreterException("Wrong argument type in \"void Print(implicit string message)\"!");
            return;
        }

        // actual printing
        std::cout << reinterpret_cast <VariableString*> (arguments[0])->value;
    }

    void Println(std::vector <Variable*>& arguments){
        if (arguments.size() != 1){
            InterpreterException("Wrong argument count in \"void Print(implicit string message)\"!");
            return;
        }
        if (arguments[0]->type() != Variable::typeString){
            InterpreterException("Wrong argument type in \"void Print(implicit string message)\"!");
            return;
        }

        // actual printing
        std::cout << reinterpret_cast <VariableString*> (arguments[0])->value << "\n";
    }
}