#ifndef PYTHON___PARSESTRUCTS_HPP
#define PYTHON___PARSESTRUCTS_HPP


#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <memory>

#include "../exceptions.hpp"
#include "../content/variables.hpp"

// do not use the base in AST tree
struct ParseStruct {
    enum {
        none, keywordIf, keywordFor, keywordWhile, keywordReturn, keywordImplicit, keywordConvert,
        variableString, variableInt, variableVariable, variableValue,
        operatorPlus, operatorMinus, operatorEqual, operatorAssign, operatorFunction, operatorMultiply, operatorDivide, operatorPower
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
    ParsePlus(std::pair <unsigned int, unsigned int> left, std::pair <unsigned int, unsigned int> right);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run(uint8_t type) const;
};

struct ParseMinus : ParseStruct {
    std::unique_ptr<ParseStruct> left, right;
    ParseMinus(std::pair <unsigned int, unsigned int> left, std::pair <unsigned int, unsigned int> right);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run(uint8_t type) const;
};

struct ParseMultiply : ParseStruct {
    std::unique_ptr<ParseStruct> left, right;
    ParseMultiply(std::pair <unsigned int, unsigned int> left, std::pair <unsigned int, unsigned int> right);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run(uint8_t type) const;
};

struct ParseDivide : ParseStruct {
    std::unique_ptr<ParseStruct> left, right;
    ParseDivide(std::pair <unsigned int, unsigned int> left, std::pair <unsigned int, unsigned int> right);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run(uint8_t type) const;
};

struct ParsePower : ParseStruct {
    std::unique_ptr<ParseStruct> left, right;
    ParsePower(std::pair <unsigned int, unsigned int> left, std::pair <unsigned int, unsigned int> right);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string run(uint8_t type) const;
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
    [[nodiscard]] Variable* run() const;
};

struct ParseInt : ParseStruct {
    std::string token;
    explicit ParseInt(const std::string& token);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] Variable* run() const;
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

struct ParseFunction : ParseStruct {
    std::string token;
    std::vector<std::unique_ptr<ParseStruct>> children;
    explicit ParseFunction(std::pair<unsigned int, unsigned int> range);
    [[nodiscard]] const uint8_t type() const override;
};





#endif //PYTHON___PARSESTRUCTS_HPP
