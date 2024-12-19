#include "stringUtils.h"

#include <string>
#include <cctype>
#include <algorithm>
#include <iostream>

std::string trimR(std::string value){
    auto iterator = find_if(value.rbegin(), value.rend(),
            [](char i){
                return !std::isspace(i);
            }).base(); // reverse iterator
    return value.substr(0, iterator - value.begin());
}

std::string trimL(std::string value){
    auto iterator = find_if(value.begin(), value.end(),
            [](char i){ // find all spaces
                return !std::isspace(i); 
            }); // and remove them
    return value.substr(iterator - value.begin());
}

std::string trim(std::string value){
    return trimL(trimR(value));
}
