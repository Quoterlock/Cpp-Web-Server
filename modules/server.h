#pragma once

#include <iostream>
#include <cstring>
#include <pthread.h>
#include <vector>
#include <mutex>
#include <signal.h>
#include <condition_variable>

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<sys/un.h>
#include<arpa/inet.h>
#include<unistd.h>

#include "serverConfig.h"

struct WorkerConfig {
    std::string pagesPath;
    std::string componentsPath;
    std::string staticFilesPath;

    WorkerConfig(std::string pagesPath, 
            std::string componentsPath, 
            std::string staticFilesPath)
        : pagesPath(pagesPath), 
        componentsPath(componentsPath),
        staticFilesPath(staticFilesPath)
    {}
};

class HttpServer {
private:
    std::condition_variable cv;
    std::vector<pthread_t> _workers;
    std::vector<int> _pendingConnections;
    std::mutex _workerMutex;
    std::mutex _consoleMutex;
    bool _stopProcessing = false;
    int _serverSocket;
    ServerConfig config;
    

    static void* clientHandlerWrapper(void* args);
    void* clientHandler();
    void initServerSocket(int port, std::string address, int maxClientsCount);
    void startWorker(const WorkerConfig& config);
public:
    HttpServer(ServerConfig config);
    ~HttpServer();

    void stop();
    void run();
};


