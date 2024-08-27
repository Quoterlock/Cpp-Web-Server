#pragma once
#include <iostream>

class StaticFilesManager {
    private:
        std::string _staticFilesPath;
    public:
        std::string getSiteIcon();
        std::string getStaticFile(std::string path);
        void setStaticFilesPath(std::string path);
};