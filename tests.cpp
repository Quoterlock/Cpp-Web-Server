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
    std::cout << result << "\n";
    printResult(result == "a bb", "string trim test");
}

void normalHttpParsingTest(){
}

void runParseHttpRequestTests(){
    normalHttpParsingTest();
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
