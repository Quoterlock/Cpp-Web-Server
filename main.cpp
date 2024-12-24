#include "router.h"
#include "logger.h"
#include "server.h"
#include "pagesManager.h"
#include "staticFilesManager.h"
#include "serverConfig.h"
#include <signal.h>

static HttpServer* server = nullptr;

void handleExit(int sig){
    if(server) {
        server->stop();
        delete server;
        server = nullptr;
    }
    exit(0);
}

int main(){
    signal(SIGINT, handleExit);
    signal(SIGTERM, handleExit);

    ServerConfig config = {
        5050, //port
        "127.0.0.1", // address
        100, // max connections
        10, // max threads

        "../pages/",
        "../pages/components/",
        "../www/"
    };

    server = new HttpServer(config);
    server->run(); // client processing loop

    return 1; // something goes wrong
}
