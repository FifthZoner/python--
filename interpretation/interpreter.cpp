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

int innerLevel = 0;

uint8_t RunLine(std::string line, unsigned long long lineNumber) {

    if (!functionStack.empty()){
        // running functions
        if (!functionStack.top().levels.empty()){
            // running local function levels, only needs to check if it's skipped or ended
            if (line == "end"){
                // going level down
                functionStack.top().levels.erase(functionStack.top().levels.end());
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
            if (innerLevel > 0){
                innerLevel--;
            }
            if (innerLevel != 0){
                return RunLineOutput::success;
            }
            if (interpreterStream->lines[globalLevels.back().recallLine].starts_with("if")){
                auto where = globalLevels.back().recallLine;

                std::unique_ptr<ParseStruct> parsed = SplitInterpreterLine(interpreterStream->lines[where], globalLevels.back().recallLine);
                if (exceptionHappened){
                    exceptionHappened = false;
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
                else {
                    if (elsePos) {
                        for (unsigned long long n = elsePos + 1; n < lineNumber; n++){
                            RunLine(interpreterStream->lines[n], n);
                        }
                    }
                    else {
                        for (unsigned long long n = where + 1; n < lineNumber; n++){
                            RunLine(interpreterStream->lines[n], n);
                        }
                    }
                }
                globalLevels.pop_back();
                interpreterStream->lines.pop_back();
                return RunLineOutput::success;
            }
            else if (interpreterStream->lines[globalLevels.back().recallLine].starts_with("while")) {
                auto where = globalLevels.back().recallLine;

                std::unique_ptr<ParseStruct> parsed = SplitInterpreterLine(interpreterStream->lines[where], globalLevels.back().recallLine);
                if (exceptionHappened){
                    exceptionHappened = false;
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
    if (line.starts_with("if")){
        if (innerLevel == 0){
            globalLevels.emplace_back(lineNumber, false);
        }
        innerLevel++;
    }
    else if (line.starts_with("while")){
        if (innerLevel == 0){
            globalLevels.emplace_back(lineNumber, false);
        }
        innerLevel++;
    }

    if (innerLevel != 0){
        return RunLineOutput::success;
    }

    std::unique_ptr<ParseStruct> parsed = SplitInterpreterLine(std::move(line), lineNumber);
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
        case ParseStruct::customFunction:
            // to avoid exception
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