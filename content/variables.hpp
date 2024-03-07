#ifndef PYTHON___VARIABLES_HPP
#define PYTHON___VARIABLES_HPP

#include <cstdint>
#include <string>
#include <memory>

struct Variable {

    [[nodiscard]] virtual const uint8_t type() const;
    [[nodiscard]] virtual std::string convert(uint8_t type) const;
    Variable* getPointer();

    enum {
        none, typeNum, typeString
    };

};

struct VariableString : Variable {
    std::string value;
    explicit VariableString(const std::string& value);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string convert(uint8_t type) const override;
};

struct VariableNum : Variable {
    long double value;
    explicit VariableNum(long double value);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::string convert(uint8_t type) const override;
};

// returns a local or global variable with given name
Variable* GetVariable(const std::string& token);


#endif //PYTHON___VARIABLES_HPP
