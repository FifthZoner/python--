#ifndef PYTHON___PMMSTDLIB_HPP
#define PYTHON___PMMSTDLIB_HPP

#include <vector>
#include <unordered_map>

#include "../function.hpp"

namespace pmmstd {
    void Print(std::vector <Variable*>& arguments);
    void Println(std::vector <Variable*>& arguments);


}

// Call this only ONCE in main before the interpreter
void PrepareStandardLibrary();

inline std::unordered_map <std::string, std::unique_ptr <Function>> functions;

#endif //PYTHON___PMMSTDLIB_HPP
