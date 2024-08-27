#include "staticFilesManager.h"

#include <fstream>
#include <iostream>

std::string readFile(std::string path){
    std::ifstream file(path);
    if (file.is_open()) {
        std::string content((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
        file.close();
        return content;
    }
    file.close();
    return "";
}

void StaticFilesManager::setStaticFilesPath(std::string path){
    _staticFilesPath = path;
}

std::string StaticFilesManager::getSiteIcon(){
    return readFile("../favicon.ico");
}

std::string StaticFilesManager::getStaticFile(std::string path){
    return readFile(_staticFilesPath + path);
}