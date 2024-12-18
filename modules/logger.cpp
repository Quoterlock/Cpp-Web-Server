#include "logger.h"
#include <iostream>
#include <fstream>

Logger::Logger(){
    _title = "Log";
}

Logger::Logger(std::string title){
    _title = title;
    _logToFile = false;
    _logToConsole = false;
}

void Logger::log(std::string msg) {
    // log in console
    if(_logToConsole)
        std::cout << "["<< _title << "] " << msg << "\n";
    
    // TODO: log in file
    if(_logToFile)
        logToFileInternal(msg); 
}

void Logger::logToFile(std::string filePath){
    _logFilePath = filePath;
    _logToFile = true;
    // TODO: check file
}

void Logger::logToConsole(){
    _logToConsole = true;
}

void Logger::logToFileInternal(const std::string &msg) {
        // Open the file in append mode
        std::ofstream logFile(_logFilePath, std::ios::app); // Open file in append mode
        
        if (logFile.is_open()) {
            logFile << "[" << _title << "] " << msg << "\n"; // Write the message to the file
            logFile.close(); // Close the file after writing
        } else {
            std::cerr << "[Logger] Cannot log to file: " << _logFilePath << "\n";
        }
}
