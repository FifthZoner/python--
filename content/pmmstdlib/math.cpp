#include <cmath>
#include <iostream>

#include "pmmstdlib.hpp"
#include "../../interpretation/exceptions.hpp"
#include "../defines.hpp"
#include "../../interpretation/checks.hpp"

// c++ standard library bindings for printing functions
namespace pmmstd {
    void IsNum(std::vector <std::string>& arguments) {
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"IsInt\" called:\n";
        #endif
        if (IsConvertibleToNum(arguments[0])){
            returnValueString = "1";
        }
        else {
            returnValueString = "0";
        }
    }

    void ToNum(std::vector <std::string>& arguments) {
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"ToNum\" called:\n";
        #endif
        returnValueString = arguments[0];
    }

    void Modulo(std::vector <std::string>& arguments) {
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"Modulo\" called:\n";
        #endif
        returnValueString = std::to_string(std::fmod(std::stold(arguments[0]) ,std::stold(arguments[1])));
    }

    void Power(std::vector <std::string>& arguments) {
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"Power\" called:\n";
        #endif
        returnValueString = std::to_string(pow(std::stod(arguments[0]), std::stod(arguments[1])));
    }
}