#ifndef PYTHON___PMM_STDLIB_HPP
#define PYTHON___PMM_STDLIB_HPP

#include <vector>
#include <unordered_map>

#include "../function.hpp"

namespace pmmstd {
    void Print(std::vector <std::string>& arguments);
    void Println(std::vector <std::string>& arguments);
    void Input(std::vector <std::string>& arguments);
}

// Call this only ONCE in main before the interpreter
void PrepareStandardLibrary();

inline std::string returnValueString;

#endif //PYTHON___PMM_STDLIB_HPP
