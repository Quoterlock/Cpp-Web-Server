#pragma once
#include "httpReq.h"
#include "httpRes.h"

class Router {
    private:
        std::string getHtmlContent(std::string path);
        httpResponse getNotFoundResponse();
        httpResponse getBadRequest(std::string msg);
        httpResponse getHtmlPageResponse(std::string pageName);
        httpResponse homePage(httpRequest request);
        httpResponse aboutPage(httpRequest req);
        httpResponse updatesPage(httpRequest req);
    public:
        httpResponse route(httpRequest request);
};
