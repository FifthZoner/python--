#ifndef PYTHON___VARIABLES_HPP
#define PYTHON___VARIABLES_HPP

#include <cstdint>
#include <string>
#include <memory>

struct Variable {

    [[nodiscard]] virtual const uint8_t type() const;
    [[nodiscard]] virtual std::unique_ptr<Variable> convert(uint8_t type) const;
    Variable* getPointer();

    enum {
        none, typeInt, typeString
    };

};

struct VariableString : Variable {
    std::string value;
    explicit VariableString(const std::string& value);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::unique_ptr<Variable> convert(uint8_t type) const override;
};

struct VariableInt : Variable {
    long long value;
    explicit VariableInt(long long value);
    [[nodiscard]] const uint8_t type() const override;
    [[nodiscard]] std::unique_ptr<Variable> convert(uint8_t type) const override;
};


#endif //PYTHON___VARIABLES_HPP
