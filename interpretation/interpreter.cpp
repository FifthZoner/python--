#include "interpreter.hpp"
#include "../parsing.hpp"
#include "../content/defines.hpp"
#include "../content/variables.hpp"
#include "exceptions.hpp"
#include "functionStack.hpp"
#include "checks.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>

extern std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

int innerLevel = 0;

void GetSwitchValue(std::string line, unsigned long long lineNumber, bool globalOrFunction) {
    SplitInterpreterLine(std::move(line), lineNumber);
    if (parsedLine.size() < 4 or parsedLine[1] != "(" or parsedLine[3] != ")") {
        ParserException("Wrong switch statement!");
    }
    std::unique_ptr<ParseValue> value = std::make_unique<ParseValue> (parsedLine[2]);
    if (globalOrFunction == 0) {
        globalLevels.back().switchValue = value->run();
    }
    else {
        functionStack.top().levels.back().switchValue = value->run();
    }
}

uint8_t RunLine(std::string line, unsigned long long lineNumber) {
    #ifdef PYTHON___DEBUG
    std::cout << "Running: " << line << "\n";
    #endif

    if (isDefiningFunction and line == "end" and innerLevel == 0 ){
            // function has been defined, huzzah!
            isDefiningFunction = false;
            PushNewFunction(lineNumber);
            return RunLineOutput::success;

    }

    if (!functionStack.empty()){
        if (innerLevel == 0 and line.starts_with("return")){
            std::unique_ptr<ParseStruct> parsed;
            try {
                parsed = SplitInterpreterLine(line, lineNumber);
            }
            catch (PMMException& e) {
                std::cout << "Parsing of current line has been cancelled!\n";
                return RunLineOutput::error;
            }

            if (parsed->type() != ParseStruct::keywordReturn){
                std::cout << "Parsing of current line has been cancelled due to invalid return statement!\n";
                return RunLineOutput::error;
            }

            try {
                reinterpret_cast <ParseReturn*> (parsed.get())->run();
            }
            catch (PMMException& e) {
                std::cout << "Interpreting of current line has been cancelled!\n";
                return RunLineOutput::error;
            }

            return RunLineOutput::returned;
        }
        // running functions
        if (!functionStack.top().levels.empty()){
            // running local function levels, only needs to check if it's skipped or ended
            if (line == "end"){

                // going level down
                if (innerLevel > 0){
                    innerLevel--;
                }
                if (innerLevel != 0){
                    return RunLineOutput::success;
                }
                if (interpreterStream->lines[functionStack.top().levels.back().recallLine].starts_with("if")){
                    auto where = functionStack.top().levels.back().recallLine;

                    std::unique_ptr<ParseStruct> parsed;
                    try {
                        parsed = SplitInterpreterLine(interpreterStream->lines[where],functionStack.top().levels.back().recallLine);
                    }
                    catch (PMMException& e) {
                        std::cout << "Parsing of current line has been cancelled!\n";
                        return RunLineOutput::error;
                    }

                    if (parsed->type() != ParseStruct::keywordIf){
                        std::cout << "Parsing of current line has been cancelled due to invalid if statement!\n";
                        return RunLineOutput::error;
                    }

                    functionStack.top().levels.pop_back();
                    interpreterStream->lines.push_back(line);

                    // looking for else
                    unsigned int elsePos = 0;
                    for (unsigned long long n = where + 1; n < lineNumber; n++){
                        if (interpreterStream->lines[n] == "else"){
                            if (elsePos != 0){
                                std::cout << "CRITICAL: Parsing of current line has been cancelled due to invalid else placement!\n";
                                interpreterStream->lines.pop_back();
                                return RunLineOutput::error;
                            }
                            elsePos = n;
                        }
                    }
                    if (reinterpret_cast <ParseIf*> (parsed.get())->run()){
                        if (elsePos){
                            for (unsigned long long n = where + 1; n < elsePos; n++){
                                RunLine(interpreterStream->lines[n], n);
                            }
                        }
                        else {
                            for (unsigned long long n = where + 1; n < lineNumber; n++){
                                RunLine(interpreterStream->lines[n], n);
                            }
                        }
                    }
                    else if (elsePos) {
                        for (unsigned long long n = elsePos + 1; n < lineNumber; n++){
                            RunLine(interpreterStream->lines[n], n);
                        }
                    }
                    functionStack.top().levels.pop_back();
                    interpreterStream->lines.pop_back();
                    return RunLineOutput::success;
                }
                else if (interpreterStream->lines[functionStack.top().levels.back().recallLine].starts_with("while")) {
                    auto where = functionStack.top().levels.back().recallLine;

                    std::unique_ptr<ParseStruct> parsed;
                    try {
                        parsed = SplitInterpreterLine(interpreterStream->lines[where], functionStack.top().levels.back().recallLine);
                    }
                    catch (PMMException& e) {
                        std::cout << "Parsing of current line has been cancelled!\n";
                        return RunLineOutput::error;
                    }

                    if (parsed->type() != ParseStruct::keywordWhile){
                        std::cout << "Parsing of current line has been cancelled due to invalid while loop!\n";
                        return RunLineOutput::error;
                    }

                    functionStack.top().levels.pop_back();
                    interpreterStream->lines.push_back(line);

                    while (reinterpret_cast <ParseWhile*> (parsed.get())->run()){
                        for (unsigned long long n = where + 1; n < lineNumber; n++){
                            // this should run all the commands inside the loop until condition is not true
                            RunLine(interpreterStream->lines[n], n);

                        }
                        functionStack.top().levels.pop_back();
                    }
                    functionStack.top().levels.pop_back();
                    interpreterStream->lines.resize(interpreterStream->lines.size() - 1);
                    return RunLineOutput::success;
                }
            }
        }
    }
    else if (!globalLevels.empty()){
        // running inside a logic statement, only needs to check if it's skipped or ended
        if (line == "end"){

            // going level down
            if (innerLevel > 0){
                innerLevel--;
            }
            if (innerLevel != 0 or isDefiningFunction){
                return RunLineOutput::success;
            }
            if (interpreterStream->lines[globalLevels.back().recallLine].starts_with("if")){
                auto where = globalLevels.back().recallLine;
                std::unique_ptr<ParseStruct> parsed;
                try {
                    parsed = SplitInterpreterLine(interpreterStream->lines[where], globalLevels.back().recallLine);
                }
                catch (PMMException& e) {
                    std::cout << "Parsing of current line has been cancelled!\n";
                    return RunLineOutput::error;
                }

                if (parsed->type() != ParseStruct::keywordIf){
                    std::cout << "Parsing of current line has been cancelled due to invalid if statement!\n";
                    return RunLineOutput::error;
                }

                globalLevels.pop_back();
                interpreterStream->lines.push_back(line);

                // looking for else
                unsigned int elsePos = 0;
                for (unsigned long long n = where + 1; n < lineNumber; n++){
                    if (interpreterStream->lines[n] == "else"){
                        if (elsePos != 0){
                            std::cout << "CRITICAL: Parsing of current line has been cancelled due to invalid else placement!\n";
                            interpreterStream->lines.pop_back();
                            return RunLineOutput::error;
                        }
                        elsePos = n;
                    }
                }
                if (reinterpret_cast <ParseIf*> (parsed.get())->run()){
                    if (elsePos){
                        for (unsigned long long n = where + 1; n < elsePos; n++){
                            RunLine(interpreterStream->lines[n], n);
                        }
                    }
                    else {
                        for (unsigned long long n = where + 1; n < lineNumber; n++){
                            RunLine(interpreterStream->lines[n], n);
                        }
                    }
                }
                else if (elsePos) {
                    for (unsigned long long n = elsePos + 1; n < lineNumber; n++){
                        RunLine(interpreterStream->lines[n], n);
                    }
                }
                globalLevels.pop_back();
                interpreterStream->lines.pop_back();
                return RunLineOutput::success;
            }
            else if (interpreterStream->lines[globalLevels.back().recallLine].starts_with("while")) {
                auto where = globalLevels.back().recallLine;

                std::unique_ptr<ParseStruct> parsed;
                try {
                    parsed = SplitInterpreterLine(interpreterStream->lines[where], globalLevels.back().recallLine);
                }
                catch (PMMException& e) {
                    std::cout << "Parsing of current line has been cancelled!\n";
                    return RunLineOutput::error;
                }

                if (parsed->type() != ParseStruct::keywordWhile){
                    std::cout << "Parsing of current line has been cancelled due to invalid while loop!\n";
                    return RunLineOutput::error;
                }

                globalLevels.pop_back();
                interpreterStream->lines.push_back(line);

                while (reinterpret_cast <ParseWhile*> (parsed.get())->run()){
                    for (unsigned long long n = where + 1; n < lineNumber; n++){
                        // this should run all the commands inside the loop until condition is not true
                        RunLine(interpreterStream->lines[n], n);

                    }
                    globalLevels.pop_back();
                }
                globalLevels.pop_back();
                interpreterStream->lines.resize(interpreterStream->lines.size() - 1);
                return RunLineOutput::success;
            }
        }
    }
    // running globally
    if (line.starts_with("if")) {
        if (innerLevel == 0){
            if (functionStack.empty()){
                globalLevels.emplace_back(lineNumber, false);
            }
            else {
                functionStack.top().levels.emplace_back(lineNumber, false);
            }
        }
        innerLevel++;
    }
    else if (line.starts_with("switch")) {
        std::cout << "Switch: " << line << "\n";
        if (innerLevel == 0){
            if (functionStack.empty()){
                try {
                    GetSwitchValue(line, lineNumber, 0);
                }
                catch (PMMException& e) {
                    std::cout << "Parsing of current line has been cancelled!\n";
                    return RunLineOutput::error;
                }

            }
            else {
                try {
                    GetSwitchValue(line, lineNumber, 1);
                }
                catch (PMMException& e) {
                    std::cout << "Parsing of current line has been cancelled!\n";
                    return RunLineOutput::error;
                }
            }
            return RunLineOutput::success;
        }
        innerLevel++;
    }
    else if (line.starts_with("while")){
        if (innerLevel == 0){
            if (functionStack.empty()){
                globalLevels.emplace_back(lineNumber, false);
            }
            else {
                functionStack.top().levels.emplace_back(lineNumber, false);
            }
        }
        innerLevel++;
    }

    if (innerLevel != 0){
        return RunLineOutput::success;
    }

    if (isDefiningFunction){
        return RunLineOutput::success;
    }

    std::unique_ptr<ParseStruct> parsed;
    try {
        parsed = SplitInterpreterLine(std::move(line), lineNumber);
    }
    catch (PMMException& e) {
        std::cout << "Parsing of current line has been cancelled!\n";
        return RunLineOutput::error;

    }


    try {
        // actual running of the code, finally, only first level things here, the rest recursively or something
        switch (parsed->type()){
            case ParseStruct::operatorAssign:
                reinterpret_cast<ParseAssign*>(parsed->getPointer())->run();
                break;
            case ParseStruct::operatorFunction:
                // no return in that case
                reinterpret_cast <ParseFunction*> (parsed.get())->run();
                break;
            case ParseStruct::customFunction:
                // to avoid exception
                break;

            default:
                InterpreterException("Wrong level 1 token!");
                break;

        }
    }
    catch (PMMException& e) {
        std::cout << "Interpreting of current line has been cancelled!\n";
        return RunLineOutput::error;
    }

    return RunLineOutput::success;
}

void RunInterpreter(InterpreterInterface* interface){
    interpreterStream = std::unique_ptr <InterpreterInterface> (interface);

    while (true) {
        std::string temp = CleanString(interpreterStream->getNextLine());
        if (temp == "exit"){
            break;
        }
        if (RunLine(temp, interpreterStream->lines.size())){
            interpreterStream->lines.push_back(temp);
            #ifdef PYTHON___DEBUG
            std::cout << "Pushed: " << interpreterStream->lines.back() << " with level: " << innerLevel<< "\n";
            #endif
        }
    }

    interpreterStream->endMessage();
}