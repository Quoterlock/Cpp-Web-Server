#include "router.h"
#include "logger.h"
#include "server.h"
#include "pagesManager.h"
#include "staticFilesManager.h"
#include "serverConfig.h"

int main(){
    ServerConfig config = {
        5050, //port
        "127.0.0.1", // address
        100, // max connections
        10, // max threads

        "../pages/",
        "../pages/components/",
        "../www/"
    };
    runServer(config);
    return 0;
}
