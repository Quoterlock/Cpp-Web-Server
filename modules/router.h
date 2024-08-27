#pragma once
#include "httpReq.h"
#include "httpRes.h"
#include "logger.h"
#include "pagesManager.h"

class Router {
    private:
        std::string _staticFilesPath;
        Logger _logger;
        HtmlRenderEngine _pages;
        httpResponse getNotFoundResponse();
        httpResponse getBadRequest(std::string msg);
        httpResponse getHtmlPageResponse(std::string pageName);
        httpResponse homePage(httpRequest request);
        httpResponse aboutPage(httpRequest req);
        httpResponse updatesPage(httpRequest req);
        std::string getStaticResource(std::string href);
    public:
        Router();
        Router(Logger logger, HtmlRenderEngine htmlRenderer);
        void setStaticFilesPath(std::string path);
        httpResponse route(httpRequest request);
};
