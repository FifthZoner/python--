#ifndef PYTHON___EXCEPTIONS_HPP
#define PYTHON___EXCEPTIONS_HPP

#include <string>
#include <exception>

class PMMException : public std::exception {
public:
    char * what ();
};



void ParserException(const std::string& content);

void InterpreterException(const std::string& content);

#endif //PYTHON___EXCEPTIONS_HPP
