#include <iostream>
#include <fstream>
#include <unordered_map>

#include "pmmstdlib.hpp"
#include "../defines.hpp"
#include "../../interpretation/exceptions.hpp"

unsigned long long handleCounter = 0;
std::unordered_map<std::string, std::unique_ptr<std::ifstream>> readFiles;
std::unordered_map<std::string, std::unique_ptr<std::ofstream>> writeFiles;

namespace pmmstd {
    void FileReadOpen(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileReadOpen\" called:\n";
        #endif
        auto file = std::make_unique<std::ifstream>();
        file->open(arguments[0]);
        if (not file->is_open()) {
            ParserException("Could not open the file!");
        }
        returnValueString = "rf" + std::to_string(++handleCounter);
        readFiles.insert(std::pair<std::string, std::unique_ptr<std::ifstream>>(returnValueString, std::move(file)));
    }

    void FileReadClose(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileReadClose\" called:\n";
        #endif
        if (auto found = readFiles.find(arguments[0]); found == readFiles.end()) {
            ParserException("Could not find the file handle!");
        }
        readFiles[arguments[0]]->close();
        readFiles.erase(arguments[0]);
    }

    void FileReadIsOpen(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileReadClose\" called:\n";
        #endif
        if (auto found = readFiles.find(arguments[0]); found == readFiles.end()) {
            ParserException("Could not find the file handle!");
        }
        if (readFiles[arguments[0]]->eof()) {
            readFiles[arguments[0]]->close();
        }
        if (readFiles[arguments[0]]->is_open()) {
            returnValueString = "1";
        }
        else {
            returnValueString = "0";
        }
    }

    void FileReadLine(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileReadLine\" called:\n";
        #endif
        if (auto found = readFiles.find(arguments[0]); found == readFiles.end()) {
            ParserException("Could not find the file handle!");
        }
        if (not std::getline(*readFiles[arguments[0]], returnValueString)) {
            ParserException("Could not read from the file! Could it be fully read already?");
        }
        if (readFiles[arguments[0]]->eof()) {
            readFiles[arguments[0]]->close();
        }
    }

    void FileReadString(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileReadString\" called:\n";
        #endif
        if (auto found = readFiles.find(arguments[0]); found == readFiles.end()) {
            ParserException("Could not find the file handle!");
        }
        if (not readFiles[arguments[0]]->is_open()) {
            ParserException("File closed or fully read already!");
        }
        *readFiles[arguments[0]] >> returnValueString;
        if (readFiles[arguments[0]]->eof()) {
            readFiles[arguments[0]]->close();
        }
    }

    void FileReadNum(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileReadNum\" called:\n";
        #endif
        if (auto found = readFiles.find(arguments[0]); found == readFiles.end()) {
            ParserException("Could not find the file handle!");
        }
        long double val = 0;
        if (not readFiles[arguments[0]]->is_open()) {
            ParserException("File closed or fully read already!");
        }
        *readFiles[arguments[0]] >> val;
        returnValueString = std::to_string(val);
        if (readFiles[arguments[0]]->eof()) {
            readFiles[arguments[0]]->close();
        }
    }

    void FileWriteOpen(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileWriteOpen\" called:\n";
        #endif
        auto file = std::make_unique<std::ofstream>();
        file->open(arguments[0]);
        if (not file->is_open()) {
            ParserException("Could not open the file!");
        }
        returnValueString = "wf" + std::to_string(++handleCounter);
        writeFiles.insert(std::pair<std::string, std::unique_ptr<std::ofstream>>(returnValueString, std::move(file)));
    }

    void FileWriteClose(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileWriteClose\" called:\n";
        #endif
        if (auto found = writeFiles.find(arguments[0]); found == writeFiles.end()) {
            ParserException("Could not find the file handle!");
        }
        writeFiles[arguments[0]]->close();
        writeFiles.erase(arguments[0]);
    }

    void FileWriteIsOpen(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileWriteClose\" called:\n";
        #endif
        if (auto found = writeFiles.find(arguments[0]); found == writeFiles.end()) {
            ParserException("Could not find the file handle!");
        }
        if (writeFiles[arguments[0]]->is_open()) {
            returnValueString = "1";
        }
        else {
            returnValueString = "0";
        }
    }

    void FileWriteLine(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileWriteLine\" called:\n";
        #endif
        if (auto found = writeFiles.find(arguments[0]); found == writeFiles.end()) {
            ParserException("Could not find the file handle!");
        }
        *writeFiles[arguments[0]] << arguments[1] << "\n";
    }

    void FileWrite(std::vector <std::string>& arguments){
        #ifdef PYTHON___DEBUG
        std::cout << "\nPMMSTDLIB function \"FileWrite\" called:\n";
        #endif
        if (auto found = writeFiles.find(arguments[0]); found == writeFiles.end()) {
            ParserException("Could not find the file handle!");
        }
        *writeFiles[arguments[0]] << arguments[1];
    }
}