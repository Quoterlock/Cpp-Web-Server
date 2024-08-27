#pragma once
#include "httpReq.h"
#include "httpRes.h"
#include "logger.h"
#include "pagesManager.h"
#include "staticFilesManager.h"

class Router {
    private:
        StaticFilesManager _files;
        Logger _logger;
        HtmlRenderEngine _pages;
        httpResponse getNotFoundResponse();
        httpResponse getBadRequest(std::string msg);
        httpResponse getHtmlPageResponse(std::string pageName);
        httpResponse homePage(httpRequest request);
        httpResponse aboutPage(httpRequest req);
        httpResponse updatesPage(httpRequest req);
        std::string getStaticResource(std::string href);
        httpResponse handleStaticFiles(std::string reqPath);
    public:
        Router();
        Router(Logger logger, HtmlRenderEngine htmlRenderer, StaticFilesManager staticFiles);
        httpResponse route(httpRequest request);
};
