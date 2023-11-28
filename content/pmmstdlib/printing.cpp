#include <iostream>

#include "pmmstdlib.hpp"
#include "../../exceptions.hpp"

// c++ standard library bindings for printing functions
namespace pmmstd {
    void Print(std::vector <Variable*>& arguments){
        std::cout << reinterpret_cast <VariableString*> (arguments[0])->value;
    }

    void Println(std::vector <Variable*>& arguments){
        std::cout << reinterpret_cast <VariableString*> (arguments[0])->value << "\n";
    }
}