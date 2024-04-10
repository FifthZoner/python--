#ifndef PYTHON___VARIABLES_HPP
#define PYTHON___VARIABLES_HPP

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

struct Variable {

    [[nodiscard]] virtual const uint8_t type() const;
    [[nodiscard]] virtual std::string convert(uint8_t type) const;
    Variable* getPointer();
    bool isArray = false;

    enum {
        none, typeNum, typeString
    };

};

struct VariableString : Variable {
    std::string value;
    std::vector <VariableString> values;
    explicit VariableString(const std::string& value);
    explicit VariableString(std::vector <std::string>& values);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string convert(uint8_t type) const override;
};

struct VariableNum : Variable {
    long double value = 0;
    std::vector <VariableNum> values;
    explicit VariableNum(long double value);
    explicit VariableNum(std::vector <long double>& values);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string convert(uint8_t type) const override;
};

// returns a local or global variable with given name
Variable* GetVariable(const std::string& token);


#endif //PYTHON___VARIABLES_HPP
