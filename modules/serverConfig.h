#pragma once
#include <string>

struct ServerConfig {
    int port;
    std::string address;
    int maxConnectionsCount;
    int maxWorkingThreadsCount;

    std::string pagesPath;
    std::string componentsPath;
    std::string staticFilesPath;
    std::string logFilePath;
};
