#include "httpReq.h"
#include "httpRes.h"
#include "router.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream> 
#include <string>
#include <cstring>
#include <unistd.h>

#include <chrono>
#include <iomanip>
#include <ctime>

/* Static methods */
std::string getContentType(const std::string& type) {
    static const std::map<std::string, std::string> contentTypeMap = {
        {"html", "text/html"},
        {"css", "text/css"},
        {"js", "application/javascript"},
        {"png", "image/png"},
        {"jpg", "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"gif", "image/gif"},
        {"json", "application/json"},
        // Add more types as needed
    };

    auto it = contentTypeMap.find(type);
    if (it != contentTypeMap.end()) {
        return it->second;
    } else {
        return "application/octet-stream"; // Default for unknown types
    }
}

httpResponse getStaticFileResponse(const std::string& content, const std::string& type) {
    httpResponse response;
    
    // Set status code and message
    response.status_code = 200;
    response.status_message = "OK";

    // Set headers
    response.headers.push_back({"Content-Type", getContentType(type)});
    response.headers.push_back({"Content-Length", std::to_string(content.size())});

    // Set body
    response.body = content;

    return response;
}

std::vector<std::string> parsePath(const std::string& path) {
    std::vector<std::string> tokens;
    std::stringstream ss(path);
    std::string token;

    // If the path starts with a '/', skip it
    if (!path.empty() && path[0] == '/') {
        ss.ignore(); // Ignore the leading '/'
    }

    while (std::getline(ss, token, '/')) {
        if (!token.empty()) { // Ignore empty tokens if any
            tokens.push_back(token);
        }
    }

    return tokens;
}

std::string getFileContent(std::string path){
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


/* Private methods */
std::string Router::getStaticResource(std::string href){
    return getFileContent(_staticFilesPath + href);
}

httpResponse Router::getNotFoundResponse(){
    httpResponse res;
    res.status_code = 404;
    res.status_message = "Not found";
    return res;
}

httpResponse Router::getBadRequest(std::string msg){
    httpResponse res;
    res.status_code = 500;
    res.status_message = "Bad request";
    res.body = msg;
    return res;
}

httpResponse Router::getHtmlPageResponse(std::string pageName){
    httpResponse res;
    res.status_code = 200;
    res.headers.emplace_back("Content-Type", "text/html");
    res.body = getFileContent(_pagesPath + pageName + ".html");
    if(res.body == "")
        return getBadRequest("Page not found");
    return res;
}

httpResponse Router::homePage(httpRequest request){
    return getHtmlPageResponse("home");
}

httpResponse Router::aboutPage(httpRequest req){
    return getHtmlPageResponse("about");
}

httpResponse Router::updatesPage(httpRequest req){
    return getHtmlPageResponse("updates");
}

/* public methods */

Router::Router(){}
Router::Router(Logger logger){
    _logger = logger;
}

void Router::setStaticFilesPath(std::string path){
    _staticFilesPath = path;
}

void Router::setPagesPath(std::string path){
    _pagesPath = path;
}

httpResponse Router::route(httpRequest request){
    auto path = request.path;

    _logger.log("Request path=" + path + ", method=" + request.method);
    
    httpResponse response;
    // handle pages
    if(strcmp(path.c_str(), "/") == 0) {
        response = homePage(request);
    }
    else if(strcmp(path.c_str(), "/about") == 0) {
        response = aboutPage(request);
    }
    else if(strcmp(path.c_str(), "/updates") == 0) {
        response = updatesPage(request);
    }
    else { 
        // handle static files
        auto splitPath = parsePath(path);
        for(auto item : splitPath){
            _logger.log("path item: " + item);
        }
        if(strcmp(splitPath[0].c_str(), "static") == 0){
            if(strcmp(splitPath[1].c_str(), "css") == 0) {
                auto body = getStaticResource("css/" + splitPath[2]);
                return getStaticFileResponse(body, "css");
            } else if(strcmp(splitPath[1].c_str(), "js") == 0) {
                auto body = getStaticResource("js/" + splitPath[2]);
                return getStaticFileResponse(body, "js");
            } else {
                response = getNotFoundResponse();
            }
        } else {
            response = getNotFoundResponse();
        }
        
    }
    return response;
}
