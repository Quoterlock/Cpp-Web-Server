#include "httpReq.h"
#include "httpRes.h"
#include "httpParser.h"
#include "stringUtils.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream> 

std::string sanitize(std::string value) {
    return trim(value);
}

httpRequest decodeHttp(std::string msg) {
    /* Message structure
     * <METHOD> <ROUTE> HTTP/1.1 \r\n
     * <HEADER>: <VALUE> \r\n
     * Cookie: <COOKIE>; <COOKIE> \r\n
     * <EMPTY LINE> \r\n
     * <BODY>
     */

    httpRequest request;

    // Split the request into lines
    std::istringstream iss(msg);
    std::string line;

    // Extract method and path from the first line
    std::getline(iss, line);
    std::istringstream firstLine(line);
    firstLine >> request.method >> request.path;  // Extract method

    // Parse headers
    while (std::getline(iss, line) && !line.empty()) {
        // check on end of headers
        if(line == "\r")
            break;

        size_t dividerPos = line.find(':');
        if (dividerPos != std::string::npos) {
            std::string key = sanitize(line.substr(0, dividerPos));
            std::string value = sanitize(line.substr(dividerPos + 1));
           
            if (key == "Cookie")
                decodeCookies(value, request);
            else 
                request.headers[key] = value;
        }
    }

    // Parse body
    std::ostringstream bodyStream;
    // combine lines with \n (except last line)
    if(std::getline(iss, line)){
        bodyStream << line;
        while (std::getline(iss, line))
            bodyStream << "\n" << line;
    }
    request.body = bodyStream.str(); // Set the body content
    
    return request;
}

void decodeCookies(std::string value, httpRequest& request) {
    std::istringstream cookiesStream(value);
    std::string cookie;
    // read cookies
    while (std::getline(cookiesStream, cookie, ';')) {
        size_t dividerPos = cookie.find('=');
        if (dividerPos!= std::string::npos) {
            // add cookie
            std::string cookieKey = sanitize(cookie.substr(0, dividerPos));
            std::string cookieValue = sanitize(cookie.substr(dividerPos + 1));
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
