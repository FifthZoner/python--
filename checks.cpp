#include "checks.hpp"

bool IsConvertibleToInt(const std::string& token) {
    for (auto current : token) {
        if (current > '9' or current < '0'){
            return false;
        }
    }
    return true;
}

bool IsGlobalVariable(const std::string& token) {

    return false;
}

bool IsFunction(const std::string& token) {

    return false;
}