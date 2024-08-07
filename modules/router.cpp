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

std::string getHtmlContent(std::string path){
    std::ifstream htmlFile(path);
    if (htmlFile.is_open()) {
        std::string content((std::istreambuf_iterator<char>(htmlFile)),
                             std::istreambuf_iterator<char>());
        htmlFile.close();
        return content;
    }
    htmlFile.close();
    return "";
}

httpResponse getNotFoundResponse(){
    httpResponse res;
    res.status_code = 404;
    res.status_message = "Not found";
    return res;
}

httpResponse getBadRequest(std::string msg){
    httpResponse res;
    res.status_code = 500;
    res.status_message = "Bad request";
    res.body = msg;
    return res;
}

httpResponse getHtmlPageResponse(std::string pageName){
    httpResponse res;
    res.status_code = 200;
    res.headers.emplace_back("Content-Type", "text/html");
    res.body = getHtmlContent("../pages/" + pageName + ".html");
    if(res.body == "")
        return getBadRequest("Page not found");
    return res;
}

httpResponse homePage(httpRequest request){
    return getHtmlPageResponse("home");
}

httpResponse aboutPage(httpRequest req){
    return getHtmlPageResponse("about");
}

httpResponse updatesPage(httpRequest req){
    return getHtmlPageResponse("updates");
}

httpResponse route(httpRequest request){
    auto path = request.path;   
    std::cout << "[Router] Request path=" 
        << path << ", method=" << request.method;

   httpResponse response;
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
        response = getNotFoundResponse();
    }
    return response;
}