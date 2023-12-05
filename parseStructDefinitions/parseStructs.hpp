#ifndef PYTHON___PARSE_STRUCTS_HPP
#define PYTHON___PARSE_STRUCTS_HPP


#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <memory>

#include "../exceptions.hpp"
#include "../content/variables.hpp"
#include "../content/function.hpp"

inline std::vector <std::string> parsedLine;

// do not use the base in AST tree
struct ParseStruct {
    enum {
        none, variableInt, variableString, variableVariable, variableValue,
        keywordIf, keywordWhile, keywordReturn, keywordImplicit, keywordEnd, keywordElse, customFunction,
        operatorPlus, operatorMinus, operatorAssign, operatorFunction, operatorMultiply, operatorDivide, operatorPower,
        operatorCompare
    };

    [[nodiscard]] virtual const uint8_t type() const;
    ParseStruct* getPointer();
};

ParseStruct* ParseMathematicalOperation(std::pair <unsigned int, unsigned int> range);

struct ParseAssign : ParseStruct {
    std::unique_ptr<ParseStruct> target;
    std::unique_ptr<ParseStruct> from;
    ParseAssign(std::pair<unsigned int, unsigned int> left, std::pair<unsigned int, unsigned int> right);
    [[nodiscard]] const uint8_t type() const override;
    void run() const;
};

struct ParsePlus : ParseStruct {
    std::unique_ptr<ParseStruct> left, right;
    uint8_t operationType;
    ParsePlus(std::pair <unsigned int, unsigned int> left, std::pair <unsigned int, unsigned int> right, uint8_t type);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run() const;
};

struct ParseMinus : ParseStruct {
    std::unique_ptr<ParseStruct> left, right;
    uint8_t operationType;
    ParseMinus(std::pair <unsigned int, unsigned int> left, std::pair <unsigned int, unsigned int> right, uint8_t type);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run() const;
};

struct ParseMultiply : ParseStruct {
    std::unique_ptr<ParseStruct> left, right;
    uint8_t operationType;
    ParseMultiply(std::pair <unsigned int, unsigned int> left, std::pair <unsigned int, unsigned int> right, uint8_t type);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run() const;
};

struct ParseDivide : ParseStruct {
    std::unique_ptr<ParseStruct> left, right;
    uint8_t operationType;
    ParseDivide(std::pair <unsigned int, unsigned int> left, std::pair <unsigned int, unsigned int> right, uint8_t type);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run() const;
};

struct ParsePower : ParseStruct {
    std::unique_ptr<ParseStruct> left, right;
    uint8_t operationType;
    ParsePower(std::pair <unsigned int, unsigned int> left, std::pair <unsigned int, unsigned int> right, uint8_t type);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run() const;
};

struct ParseValue : ParseStruct {
    std::string value;
    uint8_t valueType;
    explicit ParseValue(const std::string& value);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run() const;
};

struct ParseString : ParseStruct {
    std::string token;
    explicit ParseString(const std::string& token);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] Variable* run();
};

struct ParseInt : ParseStruct {
    std::string token;
    explicit ParseInt(const std::string& token);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] Variable* run();
};

struct ParseVariable : ParseStruct {
    std::string token;
    explicit ParseVariable(const std::string& token);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] Variable* run() const;
};

struct ParseImplicit : ParseStruct {
    std::vector <std::unique_ptr <ParseStruct>> tokens;
    explicit ParseImplicit(std::pair<unsigned int, unsigned int> range);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run(uint8_t type) const;
};

struct ParseCompare : ParseStruct {
    uint8_t compareType;
    std::unique_ptr<ParseStruct> left, right;
    explicit ParseCompare(std::pair <unsigned int, unsigned int> range);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] bool run() const;
};

struct ParseIf : ParseStruct {
    std::unique_ptr <ParseStruct> condition;
    // pass the range without if
    explicit ParseIf(std::pair <unsigned int, unsigned int> range);
    [[nodiscard]] const uint8_t type() const override;
    void run() const;
};

struct ParseWhile : ParseStruct {
    std::unique_ptr <ParseStruct> condition;
    // pass the range without while
    explicit ParseWhile(std::pair <unsigned int, unsigned int> range);
    [[nodiscard]] const uint8_t type() const override;
    bool run() const;
};

struct ParseFunction : ParseStruct {
    Function* function;
    std::vector<std::unique_ptr<ParseStruct>> tokens;
    // pass the whole function call, that is: "function(arg1, arg2, ..., argn)"
    explicit ParseFunction(std::pair<unsigned int, unsigned int> range);
    std::string run() const;
    [[nodiscard]] const uint8_t type() const override;
};

struct ParseCustomFunction : ParseStruct {
    // pass the whole line with the function definition
    explicit ParseCustomFunction(std::pair<unsigned int, unsigned int> range);
    std::string run() const;
    [[nodiscard]] const uint8_t type() const override;
};

#endif //PYTHON___PARSE_STRUCTS_HPP
