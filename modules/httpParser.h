#pragma once

#include <iostream>
#include "httpReq.h"
#include "httpRes.h"

std::string encodeResponse(httpResponse response);
httpRequest decodeHttp(std::string msg);