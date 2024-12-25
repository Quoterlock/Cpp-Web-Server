#pragma once   
#include <iostream>
#include <mutex>
#include <fstream>
class Logger {
    private:
        std::mutex _consoleMutex; 
        std::string _title;
        std::ofstream _logFile;
        bool _logToFile;
        std::string _logFilePath;
        bool _logToConsole;
        void logToFileInternal(const std::string &msg);
    public: 
        Logger();
        ~Logger();
        void log(std::string msg);
        Logger(std::string title);
        void logToFile(std::string path);
        void logToConsole();
};
