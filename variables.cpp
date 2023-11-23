#include "variables.hpp"

const uint8_t Variable::type() const {
    return Variable::none;
}
std::unique_ptr<Variable> Variable::convert(const uint8_t type) const {
    return {nullptr};
}
Variable* Variable::getPointer(){
    return this;
}

VariableString::VariableString(const std::string& value) {
    this->value = value;
}
const uint8_t VariableString::type() const {
    return Variable::typeString;
}
std::unique_ptr<Variable> VariableString::convert(const uint8_t type) const {
    return {nullptr};
}


VariableInt::VariableInt(const long long value) {
    this->value = value;
}
    const uint8_t VariableInt::type() const {
    return Variable::typeInt;
}
std::unique_ptr<Variable> VariableInt::convert(const uint8_t type) const {
    return {nullptr};
}