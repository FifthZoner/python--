#include <iostream>

#include "pmmstdlib.hpp"
#include "../../defines.hpp"

namespace pmmstd {
    void Input(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"Input\" called:\n";
        #endif
        std::getline(std::cin, returnValueString);
    }
}