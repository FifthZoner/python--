#include <iostream>

#include "pmmstdlib.hpp"
#include "../../interpretation/exceptions.hpp"
#include "../defines.hpp"

// c++ standard library bindings for printing functions
namespace pmmstd {
    void Print(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"Print\" called:\n";
        #endif
        std::cout << arguments[0];
    }

    void Println(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"Println\" called:\n";
        #endif
        std::cout << arguments[0] << "\n";
    }
}