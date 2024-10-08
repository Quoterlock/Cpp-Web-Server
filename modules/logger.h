#pragma once   
#include <iostream>

class Logger {
    private:
        std::string _title;
        bool _logToFile;
        std::string _logFilePath;
        bool _logToConsole;
    public: 
        Logger();
        void log(std::string msg);
        Logger(std::string title);
        void logToFile(std::string path);
        void logToConsole();
};