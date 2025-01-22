#include "httpReq.h"
#include "httpRes.h"
#include "httpParser.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream> 

httpRequest decodeHttp(std::string msg) {
    httpRequest request;

    // Split the request into lines
    std::istringstream iss(msg);
    std::string line;

    // Extract method and path from the first line
    std::getline(iss, line);
    std::istringstream firstLine(line);
    firstLine >> request.method >> request.path;  // Extract method

    // TODO: Fix vulnerable code
    // sanitize and validate values
    // Parse headers
    while (std::getline(iss, line)) {
        // Remove potential trailing '\r'
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Stop parsing headers on a blank line
        if (line.empty()) {
            break;
        }

        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            // TODO : skip ":" then trim
            std::string value = line.substr(pos + 2); // Skip ': ' after key

            // Insert or update header
            request.headers[key] = value;

            // Parse cookies if present
            if (key == "Cookie") {
                decodeCookies(value, request);
            }
        }
    }

    // Parse body
    // The body might be empty or span multiple lines, adjust accordingly
    std::ostringstream bodyStream;
    while (std::getline(iss, line)) {
        // Remove potential trailing '\r'
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        bodyStream << line << '\n';
    }
    request.body = bodyStream.str();

    return request;
}

void decodeCookies(std::string value, httpRequest& request) {
    std::istringstream cookiesStream(value);
    std::string cookie;
    while (std::getline(cookiesStream, cookie, ';')) {
        size_t eqPos = cookie.find('=');
        if (eqPos != std::string::npos) {
            std::string cookieKey = cookie.substr(0, eqPos);
            std::string cookieValue = cookie.substr(eqPos + 1);
            // Trim whitespace
            cookieKey.erase(cookieKey.find_last_not_of(" \t") + 1);
            cookieValue.erase(0, cookieValue.find_first_not_of(" \t"));
            request.cookies[cookieKey] = cookieValue;
        }
    }
}

std::string encodeResponse(httpResponse response){
    std::ostringstream oss;

    // Write status line
    oss << "HTTP/1.1 " << response.status_code << " " << response.status_message << "\r\n";

    // Write headers
    for (const auto& header : response.headers) {
        oss << header.first << ": " << header.second << "\r\n";
    }

    // Write body
    if (!response.body.empty()) {
        oss << "\r\n" << response.body;
    }

    return oss.str();

}
