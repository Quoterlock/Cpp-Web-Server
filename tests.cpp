#include "stringUtils.h"
#include "httpReq.h"
#include "httpParser.h"

#include<iostream>

void printResult(bool result, std::string test)
{
    if(result)
        std::cout << "(+) Test \"" << test << "\" Succeed\n";
    else
        std::cout << "(!) Test \"" << test << "\" Failed\n";
}

void trimTest(){
    std::string result = trim("  a bb  ");
    printResult(result == "a bb", "string trim test");
}

// Helper function to print httpRequest
void printHttpRequest(const httpRequest& req) {
    std::cout << "Method: " << req.method << "\n";
    std::cout << "Path: " << req.path << "\n";
    std::cout << "Headers:\n";
    for (const auto& header : req.headers) {
        std::cout << "  " << header.first << ": " << header.second << "\n";
    }
    std::cout << "Body: " << req.body << "\n";
    std::cout << "Cookies:\n";
    for (const auto& cookie : req.cookies) {
        std::cout << "  " << cookie.first << ": " << cookie.second << "\n";
    }
}

void requestHeadersParsingTest(){
    std::string msg ="POST /route HTTP/1.1\r\n";
    msg +="Host: developer.mozilla.org\r\n";
    msg +="User-Agent: curl/8.6.0\r\n";
    msg +="Accept: */*\r\n";
    msg +="Cookie: my_cookie=some_value; my_another_cookie=some_value2\r\n";
    msg +="Content-Type: application/json\r\n";
    msg +="Content-Length: 345\r\n";
    msg +="\r\n";
    // body
    msg +="{\r\n\"data\":\"ABC\"\r\n}";

    httpRequest expected;
    expected.method = "POST";
    expected.path = "/route";
    expected.body ="{\r\n\"data\":\"ABC\"\r\n}"; 
    expected.headers["Host"] = "developer.mozilla.org";
    expected.headers["User-Agent"] = "curl/8.6.0";
    expected.headers["Accept"] ="*/*";
    expected.headers["Content-Type"]="application/json";
    expected.headers["Content-Length"]="345";
    expected.cookies["my_cookie"]="some_value";
    expected.cookies["my_another_cookie"]="some_value2";
    
    // act
    httpRequest actual = decodeHttp(msg);
     printHttpRequest(actual); // function for debugging
    printResult(actual == expected, "request message parsing test 1");
}

void runParseHttpRequestTests(){
    requestHeadersParsingTest();
}

void runStringUtilsTests(){
    trimTest(); 
}

int main(){
    //TODO: move tests to a separate lib
    runStringUtilsTests(); 
    runParseHttpRequestTests();
    return 0;
}
