#include "logger.h"
#include <iostream>

Logger::Logger(){
    _title = "Log";
}

Logger::Logger(std::string title){
    _title = title;
}

void Logger::log(std::string msg) {
    // log in console
    if(_logToConsole)
        std::cout << "["<< _title << "]:" << msg << "\n";
    
    // TODO: log in file
    if(_logToFile)
        std::cout << "Cannot log to file";
}

void Logger::logToFile(std::string filePath){
    _logFilePath = filePath;
    _logToFile = true;
    // TODO: check file
}

void Logger::logToConsole(){
    _logToConsole = true;
}