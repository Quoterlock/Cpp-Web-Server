#include "logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

Logger::Logger(): _logFilePath("Log.txt"), _title("Server log"){
}

Logger::~Logger(){
    if(_logFile.is_open())
        _logFile.close();
}

static std::string getTimeStamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Logger::log(std::string msg) {
    auto timestamp = getTimeStamp();
    {
        std::lock_guard<std::mutex> lock(_consoleMutex);
        if(_logToConsole)
            std::cout << "["<< _title << "]:["<< timestamp <<"] " << msg << "\n";
        if(_logToFile)
            logToFileInternal(msg); 
    }
}

void Logger::logToFile(std::string filePath){
    _logFilePath = filePath;
    _logFile.open(_logFilePath, std::ios::app);
    _logToFile = true;
}

void Logger::logToConsole(){
    _logToConsole = true;
}

void Logger::logToFileInternal(const std::string &msg) {
        // Open the file in append mode
        std::ofstream logFile(_logFilePath, std::ios::app); // Open file in append mode
        
        if (logFile.is_open()) {
            logFile << "[" << _title << "] " << msg << "\n"; // Write the message to the file
        } else {
            std::cerr << "[Logger] Cannot log to file: " << _logFilePath << "\n";
        }
}
