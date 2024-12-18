#include "router.h"
#include "logger.h"
#include "server.h"
#include "pagesManager.h"
#include "staticFilesManager.h"

int main(){
    // config
    Logger logger("Server");
    logger.logToConsole();
    logger.logToFile("log.txt");

    HtmlRenderEngine pages;
    pages.setPagesPath("../pages/");
    pages.setComponentsPath("../pages/components/");

    StaticFilesManager files;
    files.setStaticFilesPath("../www/");

    Router router(logger, pages, files);

    // TODO: Add error handling
    HttpServer server = HttpServer(logger, router);
    server.setPort(8080);
    server.run();
    
    return 0;
}
