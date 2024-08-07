#ifndef HTTP_RES_H
#define HTTP_RES_H

#include <string>
#include <vector>

struct httpResponse {
    int status_code;                    // HTTP status code (e.g., 200, 404)
    std::string status_message;         // HTTP status message (e.g., OK, Not Found)
    std::vector<std::pair<std::string, std::string>> headers;  // Response headers
    std::string body;                   // Response body
};

#endif