#ifndef PYTHON___PARSING_HPP
#define PYTHON___PARSING_HPP

#include <vector>
#include <string>

#include "parseStructs.hpp"

std::vector <std::vector <std::string>> ParseCommandArguments(const int argc, char** argv);

// returns a vector of strings containing tokens, keywords, values and operators
std::vector <std::string> SplitInterpreterLine(std::string line);

ParseStruct* ParseLine(std::vector <std::string> line);

#endif //PYTHON___PARSING_HPP
