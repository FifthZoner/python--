#include "pmmstdlib.hpp"
#include "../../interpretation/exceptions.hpp"

namespace pmmstd {
    void System(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"System\" called:\n";
        #endif
        if (arguments.empty()) {
            ParserException("Somehow passed an empty array to system function!");
        }
        std::system(arguments[0].c_str());
    }
}