#pragma once
#include "httpReq.h"
#include "httpRes.h"
#include "logger.h"

class Router {
    private:
        std::string _staticFilesPath;
        std::string _pagesPath;
        Logger _logger;
        httpResponse getNotFoundResponse();
        httpResponse getBadRequest(std::string msg);
        httpResponse getHtmlPageResponse(std::string pageName);
        httpResponse homePage(httpRequest request);
        httpResponse aboutPage(httpRequest req);
        httpResponse updatesPage(httpRequest req);
        std::string getStaticResource(std::string href);
    public:
        Router();
        Router(Logger logger);
        void setStaticFilesPath(std::string path);
        void setPagesPath(std::string path);
        httpResponse route(httpRequest request);
};
