#ifndef PYTHON___PMM_STDLIB_HPP
#define PYTHON___PMM_STDLIB_HPP

#include <vector>
#include <unordered_map>

#include "../function.hpp"

namespace pmmstd {
    void Print  (std::vector <std::string>& arguments);
    void Println(std::vector <std::string>& arguments);
    void Input  (std::vector <std::string>& arguments);
    void IsNum  (std::vector <std::string>& arguments);
    void ToNum  (std::vector <std::string>& arguments);
    void Modulo (std::vector <std::string>& arguments);
    void Power  (std::vector <std::string>& arguments);
    void System (std::vector <std::string>& arguments);
    void FileReadOpen   (std::vector <std::string>& arguments);
    void FileReadClose  (std::vector <std::string>& arguments);
    void FileReadIsOpen (std::vector <std::string>& arguments);
    void FileReadLine   (std::vector <std::string>& arguments);
    void FileReadString (std::vector <std::string>& arguments);
    void FileReadNum    (std::vector <std::string>& arguments);
    void FileWriteOpen  (std::vector <std::string>& arguments);
    void FileWriteClose (std::vector <std::string>& arguments);
    void FileWriteIsOpen(std::vector <std::string>& arguments);
    void FileWriteLine  (std::vector <std::string>& arguments);
    void FileWrite      (std::vector <std::string>& arguments);
}

// Call this only ONCE in main before the interpreter
void PrepareStandardLibrary();

inline std::string returnValueString;

#endif //PYTHON___PMM_STDLIB_HPP
