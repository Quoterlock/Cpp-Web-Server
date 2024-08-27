#include "router.h"
#include "logger.h"
#include "server.h"
#include "pagesManager.h"

int main(){
    Logger logger("Server");
    logger.logToConsole();

    HtmlRenderEngine pages;
    pages.setPagesPath("../pages/");
    pages.setComponentsPath("../pages/components/");

    Router router(logger, pages);
    router.setStaticFilesPath("../www/");

    // TODO: Add error handling
    HttpServer server = HttpServer(logger, router);
    server.setPort(8080);
    server.run();
    
    return 0;
}
