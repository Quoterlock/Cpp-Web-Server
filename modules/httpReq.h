#ifndef HTTP_REQ_H
#define HTTP_REQ_H

#include <string>
#include <map>

struct httpRequest {
    std::string method;                 // HTTP method (e.g., GET, POST)
    std::string path;                   // Request path
    std::map<std::string, std::string> headers;  // Request headers (using std::map)
    std::string body;                   // Request body
    std::map<std::string, std::string> cookies;  // Cookies
};

#endif