#pragma once   
#include <iostream>

class HtmlRenderEngine {
    private:
        std::string _pagesPath;
        std::string _componentsPath;
        std::string getPageContent(std::string path);
    public:
        std::string getPage(std::string name);
        void setComponentsPath(std::string path);
        void setPagesPath(std::string path);
};