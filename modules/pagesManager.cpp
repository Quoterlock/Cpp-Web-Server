#include "pagesManager.h"
#include <regex>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

void HtmlRenderEngine::setComponentsPath(std::string path){
    _componentsPath = path;
}

void HtmlRenderEngine::setPagesPath(std::string path){
    _pagesPath = path;
}

std::string HtmlRenderEngine::getPageContent(std::string path){
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

std::string HtmlRenderEngine::getPage(std::string name){
    auto pageContent = getPageContent(_pagesPath + name + ".html");
    std::regex pattern(R"(\@\{(\w+)})");
    std::smatch match;

    while(std::regex_search(pageContent, match, pattern)){
        std::string componentName = match[1].str();
        std::string componentContent = getPageContent(_componentsPath + componentName + ".html");

        if(!componentContent.empty()){
            pageContent.replace(match.position(0), match.length(0), componentContent);
        } else {
            pageContent.replace(match.position(0), match.length(0), "");
        }
    }
    return pageContent;
}