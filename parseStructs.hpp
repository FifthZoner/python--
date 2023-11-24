#ifndef PYTHON___PARSESTRUCTS_HPP
#define PYTHON___PARSESTRUCTS_HPP


#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <memory>

#include "exceptions.hpp"
#include "variables.hpp"

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
        none, keywordIf, keywordFor, keywordWhile, keywordReturn, keywordImplicit, keywordConvert,
        variableString, variableInt, variableVariable, variableValue,
        operatorPlus, operatorMinus, operatorEqual, operatorAssign, operatorFunction, operatorEquation
    };

    virtual const uint8_t type() const;
    ParseStruct* getPointer();
};

struct ParseAssign : ParseStruct {
    std::unique_ptr<ParseStruct> target;
    std::unique_ptr<ParseStruct> from;
    ParseAssign(std::pair<unsigned int, unsigned int> left, std::pair<unsigned int, unsigned int> right);
    [[nodiscard]] const uint8_t type() const override;
    void run();
};

struct ParsePlus : ParseStruct {
    std::unique_ptr<ParseStruct> left, right;
    ParsePlus(const std::string& left, const std::string& right);
    ParsePlus(ParseStruct* left, const std::string& right);
    ParsePlus(const std::string& left, ParseStruct* right);
    [[nodiscard]] const uint8_t type() const override;
    std::string run(uint8_t type);
};

struct ParseValue : ParseStruct {
    std::string value;
    uint8_t valueType;
    explicit ParseValue(const std::string& value);
    [[nodiscard]] const uint8_t type() const override;
    std::string run() const;
};

struct ParseEquation : ParseStruct {
    std::unique_ptr<ParseStruct> from;
    explicit ParseEquation(std::pair<unsigned int, unsigned int> range);
    // use this with implicit only
    [[nodiscard]] const uint8_t type() const override;
    std::string run(uint8_t type) const;
};

struct ParseString : ParseStruct {
    std::string token;
    explicit ParseString(const std::string& token);
    [[nodiscard]] const uint8_t type() const override;
};

struct ParseInt : ParseStruct {
    std::string token;
    explicit ParseInt(const std::string& token);
    [[nodiscard]] const uint8_t type() const override;
    Variable* run() const;
};

struct ParseVariable : ParseStruct {
    std::string token;
    explicit ParseVariable(const std::string& token);
    [[nodiscard]] const uint8_t type() const override;
    Variable* run() const;
};

struct ParseImplicit : ParseStruct {
    std::vector <std::unique_ptr <ParseStruct>> tokens;
    explicit ParseImplicit(std::pair<unsigned int, unsigned int> range);
    [[nodiscard]] const uint8_t type() const override;
};

struct ParseFunction : ParseStruct {
    std::string token;
    std::vector<std::unique_ptr<ParseStruct>> children;
    explicit ParseFunction(std::pair<unsigned int, unsigned int> range);
    [[nodiscard]] const uint8_t type() const override;
};





#endif //PYTHON___PARSESTRUCTS_HPP
