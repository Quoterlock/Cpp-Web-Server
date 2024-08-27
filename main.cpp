#include "router.h"
#include "logger.h"
#include "server.h"

int main(){
    Logger logger = Logger("Server");
    logger.logToConsole();

    Router router = Router(logger);
    router.setPagesPath("../pages/");
    router.setStaticFilesPath("../www/");

    // TODO: Add error handling
    HttpServer server = HttpServer(logger, router);
    server.setPort(8080);
    server.run();
    
    return 0;
}
