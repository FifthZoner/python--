#ifndef PYTHON___PARSING_HPP
#define PYTHON___PARSING_HPP

#include <vector>
#include <string>
#include <cstdint>

#include "parseStructs.hpp"

std::vector <std::vector <std::string>> ParseCommandArguments(int argc, char** argv);

// returns a vector of strings containing tokens, keywords, values and operators
std::unique_ptr<ParseStruct> SplitInterpreterLine(std::string line);

uint8_t CheckTokenType(const std::string& token);

#endif //PYTHON___PARSING_HPP
