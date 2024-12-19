#ifndef HTTP_REQ_H
#define HTTP_REQ_H

#include <iostream>
#include <string>
#include <map>

struct httpRequest {
    std::string method;                 // HTTP method (e.g., GET, POST)
    std::string path;                   // Request path
    std::map<std::string, std::string> headers;  // Request headers (using std::map)
    std::string body;                   // Request body
    std::map<std::string, std::string> cookies;  // Cookies
    
    bool operator==(const httpRequest& other) const {
        return method == other.method &&
            path == other.path &&
            headers == other.headers &&
            body == other.body &&
            cookies == other.cookies;
    }
};

#endif
