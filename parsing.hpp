#ifndef PYTHON___PARSING_HPP
#define PYTHON___PARSING_HPP

#include <vector>
#include <string>
#include <cstdint>

#include "parseStructDefinitions/parseStructs.hpp"

std::vector <std::vector <std::string>> ParseCommandArguments(int argc, char** argv);

std::unique_ptr<ParseStruct> ParseLine(std::pair<unsigned int, unsigned int> range, unsigned long long lineNumber);

// returns a vector of strings containing tokens, keywords, values and operators
void SplitInterpreterLine(std::string line, unsigned long long lineNumber);

uint8_t CheckTokenType(const std::string& token);

#endif //PYTHON___PARSING_HPP
