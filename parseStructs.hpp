#ifndef PYTHON___PARSESTRUCTS_HPP
#define PYTHON___PARSESTRUCTS_HPP


#include <cstdint>
#include <string>
#include <vector>

// allows for declarations like this

// def func(int var1, string var2):
// to be split into
// keywordDef(func) -> {variableInt(var1), variableString(var2)}

// def func(implicit string var):
// to be split into
// keywordDef(func) -> keywordImplicit() -> variableString(var) (this converts all types with their string conversion method)

// do not use
struct ParseStruct {
    enum {
        none, keywordIf, keywordFor, keywordWhile, keywordReturn, keywordDef, keywordImplicit, keywordConvert, variableString, variableInt, variableUnknown, variableValue, operatorPlus, operatorMinus, operatorEqual, operatorAssign, operatorFunction
    };

    virtual const uint8_t type() const;
};

struct ParseAssign : ParseStruct {
    ParseStruct* target;
    std::vector<ParseStruct*> tokens;
    ParseAssign(std::string left, std::string right);
    const uint8_t type() const;
};

struct ParseValue : ParseStruct {
    std::string token;
    ParseValue(std::string string);
    const uint8_t type() const;
};

struct ParseUnknown : ParseStruct {
    std::string token;
    ParseUnknown(std::string string);
    const uint8_t type() const;
};

struct ParseFunction : ParseStruct {
    std::string token;
    std::vector<ParseStruct*> children;
    ParseFunction(std::string string);
    const uint8_t type() const;

};





#endif //PYTHON___PARSESTRUCTS_HPP
