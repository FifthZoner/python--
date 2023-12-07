#include <iostream>

#include "interpreterInterface.hpp"

std::string InterpreterInterface::getNextLine() {
    return "exit";
}
void InterpreterInterface::endMessage() {}

InterpreterFromUser::InterpreterFromUser() {
    std::cout << "Welcome to Python-- interpreter interface!\n";
}

std::string InterpreterFromUser::getNextLine() {
    // TODO: add support for indentations, not needed - for now
    std::cout << "> ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}
void InterpreterFromUser::endMessage() {
    std::cout << "Goodbye!" << std::endl;
}

InterpreterFromFile::InterpreterFromFile(std::vector <std::string> files) {
    this->files = std::move(files);
    std::cout << "Interpreting following files:\n";
    for (auto& n : this->files){
        std::cout << n << "\n";
    }
    std::cout << ":\n";
}
std::string InterpreterFromFile::getNextLine() {
    if (files.empty()){
        return "exit";
    }

    if (!file.is_open()){
        if (currentFile != 0){
            return "exit";
        }
        for (; currentFile < files.size(); currentFile++){
            file.open(files[currentFile]);
            if (file.is_open()){
                break;
            }
            std::cout << "\n\nCould not open file: " << files[currentFile] << "!\n\n";
        }
        if (!file.is_open()){
            return "exit";
        }
    }

    if (file.eof()){
        file.close();
        currentFile++;
        for (; currentFile < files.size(); currentFile++){
            file.open(files[currentFile]);
            if (file.is_open()){
                break;
            }
            std::cout << "\n\nCould not open file: " << files[currentFile] << "!\n\n";
        }
        if (!file.is_open()){
            return "exit";
        }
    }

    std::string input;
    std::getline(file, input);

    if (input == ""){
        return getNextLine();
    }

    return std::move(input);
}
void InterpreterFromFile::endMessage() {
    std::cout << "\nInterpretation finished!\n\n";
}