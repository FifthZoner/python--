#ifndef PYTHON___VARIABLES_HPP
#define PYTHON___VARIABLES_HPP

#include <cstdint>
#include <string>

struct Variable {
    std::string name;
    Variable(std::string name);
    virtual const uint8_t type() const;
    virtual Variable* convert(const uint8_t type);

    enum {
        none, typeInt, typeString
    };

};

struct VariableString : Variable {
    std::string value;
    VariableString(std::string name, std::string value);
};

struct VariableInt : Variable {
    long long value;
    VariableInt(std::string name, long long value);
};


#endif //PYTHON___VARIABLES_HPP
