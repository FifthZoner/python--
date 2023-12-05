#include "interpreter.hpp"
#include "../parsing.hpp"
#include "../defines.hpp"
#include "../content/variables.hpp"
#include "../exceptions.hpp"
#include "../functionStack.hpp"
#include "../checks.hpp"

#include <iostream>
#include <unordered_map>
#include <utility>

extern std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

bool exceptionHappened = false;

uint8_t RunLine(std::string line, unsigned long long lineNumber) {

    if (interpreterState == InterpreterState::stateNormal){
        if (!functionStack.empty()){
            // running functions
            if (!functionStack.top().levels.empty()){
                // running local function levels, only needs to check if it's skipped or ended
                if (line == "end"){
                    // going level down
                    functionStack.top().levels.erase(functionStack.top().levels.end());
                    return RunLineOutput::success;
                }
                if (line == "else"){
                    if (!interpreterStream->lines[functionStack.top().levels.back().recallLine].starts_with("if")) {
                        InterpreterException("Else statement ending while!");
                        return RunLineOutput::error;
                    }

                    bool pervious = functionStack.top().levels.back().isRunning;
                    functionStack.top().levels.erase(functionStack.top().levels.end());
                    if (pervious) {
                        functionStack.top().levels.emplace_back(interpreterStream->lines.size() - 1, false);
                        std::cout << "False else\n";
                    }
                    else {
                        functionStack.top().levels.emplace_back(interpreterStream->lines.size() - 1, true);
                        std::cout << "True else\n";
                    }
                    return RunLineOutput::success;
                }
                if (!functionStack.top().levels.back().isRunning){
                    // skipped
                    return RunLineOutput::success;
                }
            }
        }

        if (!globalLevels.empty()){
            // running inside a logic statement, only needs to check if it's skipped or ended
            if (line == "end"){
                // going level down
                if (interpreterStream->lines[globalLevels[globalLevels.size() - 1].recallLine].starts_with("if")){
                    globalLevels.erase(globalLevels.end());
                    return RunLineOutput::success;
                }
                else if (interpreterStream->lines[globalLevels[globalLevels.size() - 1].recallLine].starts_with("while")) {
                    auto where = globalLevels.back().recallLine;

                    std::unique_ptr<ParseStruct> parsed = SplitInterpreterLine(std::move(interpreterStream->lines[where]));
                    if (exceptionHappened){
                        exceptionHappened = false;
                        std::cout << "Parsing of current line has been cancelled!\n";
                        return RunLineOutput::error;
                    }

                    if (parsed->type() != ParseStruct::keywordWhile){
                        std::cout << "Parsing of current line has been cancelled due to invalid while loop!\n";
                        return RunLineOutput::error;
                    }

                    globalLevels.erase(globalLevels.end());

                    while (reinterpret_cast <ParseWhile*> (parsed.get())->run()){
                        for (unsigned long long n = where + 1; n < lineNumber; n++){
                            // this should run all the commands inside the loop until condition is not true
                            RunLine(interpreterStream->lines[n], n);
                        }
                    }
                    globalLevels.erase(globalLevels.end());
                    return RunLineOutput::success;
                }
            }
            if (line == "else"){
                if (!interpreterStream->lines[globalLevels[globalLevels.size() - 1].recallLine].starts_with("if")) {
                    std::cout << interpreterStream->lines[globalLevels[globalLevels.size() - 1].recallLine] << "\n";
                    InterpreterException("Else statement ending while!");
                    return RunLineOutput::error;
                }

                bool before = globalLevels.back().isRunning;
                globalLevels.erase(globalLevels.end());
                if (before) {
                    globalLevels.emplace_back(interpreterStream->lines.size() - 1, false);
                    std::cout << "False else\n";
                }
                else {
                    globalLevels.emplace_back(interpreterStream->lines.size() - 1, true);
                    std::cout << "True else\n";
                }
                return RunLineOutput::success;
            }
            if (!globalLevels.back().isRunning){
                // skipped
                return RunLineOutput::success;
            }
        }
        // running globally

        std::unique_ptr<ParseStruct> parsed = SplitInterpreterLine(std::move(line));
        if (exceptionHappened){
            exceptionHappened = false;
            std::cout << "Parsing of current line has been cancelled!\n";
            return RunLineOutput::error;
        }

        // actual running of the code, finally, only first level things here, the rest recursively or something
        switch (parsed->type()){
            case ParseStruct::operatorAssign:
                reinterpret_cast<ParseAssign*>(parsed->getPointer())->run();
                break;
            case ParseStruct::operatorFunction:
                // no return in that case
                reinterpret_cast <ParseFunction*> (parsed.get())->run();
                break;
            case ParseStruct::keywordIf:
                // no return in that case
                reinterpret_cast <ParseIf*> (parsed.get())->run();
                break;
            case ParseStruct::keywordWhile:
                // no return in that case
                reinterpret_cast <ParseWhile*> (parsed.get())->run();
                break;

            default:
                InterpreterException("Wrong level 1 token!");
                break;

        }

        if (exceptionHappened){
            exceptionHappened = false;
            std::cout << "Interpreting of current line has been cancelled!\n";
            return RunLineOutput::error;
        }
    }
    else {
        // saving a function
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
            std::cout << "Pushed: " << interpreterStream->lines.back() << "\n";
            #endif
        }
    }

    interpreterStream->endMessage();
}