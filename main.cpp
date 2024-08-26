#include "router.h"
#include "logger.h"
#include "server.h"

int main(){
    Logger logger = Logger("Server");
    logger.logToConsole();

    Router router = Router();

    // TODO: Add error handling
    HttpServer server = HttpServer(logger, router);
    server.run();
    
    return 0;
}
