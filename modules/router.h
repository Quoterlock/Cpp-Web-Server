#pragma once
#include "httpReq.h"
#include "httpRes.h"
#include "logger.h"
#include "pagesManager.h"
#include "staticFilesManager.h"

class Router {
    private:
        StaticFilesManager _files;
        Logger& _logger;
        HtmlRenderEngine _pages;
        
        // custom entrypoints
        httpResponse jsonPage(httpRequest request);
        httpResponse homePage(httpRequest request);
        httpResponse aboutPage(httpRequest req);
        httpResponse updatesPage(httpRequest req);
        
        // default entrypoints
        httpResponse getOkResponse(std::string msg);
        httpResponse getServerSideErrorResponse(std::string msg);
        httpResponse getNotFoundResponse();
        httpResponse getBadRequest(std::string msg);
        httpResponse getHtmlPageResponse(std::string pageName);
        httpResponse handleStaticFiles(std::string reqPath);
        
        std::string getStaticResource(std::string href);
    public:
        Router(Logger& logger, HtmlRenderEngine& htmlRenderer,
                StaticFilesManager& staticFiles);
        httpResponse route(httpRequest request);
};
