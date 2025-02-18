#include "server.h"
#include "logger.h"
#include "httpParser.h"
#include "router.h"
#include "serverConfig.h"

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

void* HttpServer::clientHandler(){
    Router router(_logger, _pages, _files);

    // do the work
    while(true){
        int clientSocket = -1;
        // get a job
        {
            std::unique_lock<std::mutex> lock(_workerMutex);
            cv.wait(lock, [this]{
                return !_pendingConnections.empty() || _stopProcessing;
            });
            if(_stopProcessing)
                break;

            clientSocket = _pendingConnections.back();
            _pendingConnections.pop_back();
            _logger.log("Worker: Take client socket" + std::to_string(clientSocket));
        }
        
        // handle client connection
        const int BUFFER_SIZE = 4096;
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if(bytesReceived == -1){
            _logger.log("Error receiving message!");
            close(clientSocket);
            continue; // get next task
        } else if(bytesReceived == 0) {
            _logger.log("Client closed a connection");
            close(clientSocket);
            continue;
        }
        _logger.log(buffer);
    
        // process message
        auto request = decodeHttp(buffer);
        auto response = router.route(request);
        std::string responseStr = encodeResponse(response);
    
        // send a response
        int bytesSent = send(clientSocket, responseStr.c_str(), 
            strlen(responseStr.c_str()), 0);            

        if(bytesSent == -1) {
            _logger.log("Error sending response");
            close(clientSocket);
            continue; // get next task
        }
        _logger.log("Worker: close client socket");
        close(clientSocket);
    }
    return nullptr;
}

HttpServer::~HttpServer(){
    _logger.log("Desctructor"); 
}

void HttpServer::stop(){
    _logger.log("Stop execution");
    {
        std::lock_guard<std::mutex> lock(_workerMutex);
        _stopProcessing = true;
    }
    cv.notify_all();

    // wait will all is done
    for(auto worker : _workers){
        _logger.log("Join worker thread");
        pthread_join(worker, NULL); 
    }

    for(auto socket : _pendingConnections){
        _logger.log("Close client socket");
        close(socket); 
    }

    _logger.log("Clear vectors...");
    _pendingConnections.clear();
    _workers.clear();
    _logger.log("Close server socket");
    close(_serverSocket);
}

void HttpServer::initServerSocket(int port, std::string address, int maxClientsCount){
    // create socket
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverSocket == -1) {
        _logger.log("Error creating socket!");
        exit(EXIT_FAILURE);
    }

    // config address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // use IPv4
    serverAddr.sin_port = htons(port);
    if(inet_aton(address.c_str(), &serverAddr.sin_addr) <= 0){
        _logger.log("Error converting an address!");
        exit(EXIT_FAILURE);
    }
    // bind socket
    if(bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
        _logger.log("Error binding server socket");
        exit(EXIT_FAILURE);
    }

    // set timeout (afk mode)
    struct timeval timeout;
    timeout.tv_sec = 60;  // Timeout of 20 seconds
    timeout.tv_usec = 0;
    setsockopt(_serverSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    

    // set listener
    if(listen(_serverSocket, maxClientsCount) == -1) {
        _logger.log("Error listening!");
    } else {
        _logger.log("Listening port " + std::to_string(port)); 
    }
}

void* HttpServer::clientHandlerWrapper(void* args) {
    HttpServer* server = static_cast<HttpServer*>(args);
    return server->clientHandler();  // Call the non-static member function
}

void HttpServer::startWorker(const WorkerConfig& config){
    _logger.log("Start worker");
    pthread_t thread;
    WorkerConfig* configPtr = new WorkerConfig(config);
    pthread_create( &thread, NULL, HttpServer::clientHandlerWrapper, (void*)this);
    _workers.push_back(thread);
}


HttpServer::HttpServer(ServerConfig config){
    this->_config = config;

    // init
    _logger.logToConsole();
    _logger.logToFile(config.logFilePath);
    _pages.setPagesPath(config.pagesPath);
    _pages.setComponentsPath(config.componentsPath);
    _files.setStaticFilesPath(config.staticFilesPath);

}

void HttpServer::run(){
    //init server socket
    initServerSocket(_config.port, _config.address, _config.maxConnectionsCount);

    WorkerConfig workerConfig = {
        _config.pagesPath,
        _config.componentsPath,
        _config.staticFilesPath
    };

    // init workers
    for(int i = 0; i < _config.maxWorkingThreadsCount; i++){
        startWorker(workerConfig); 
    }

    // accept clients
    while(true){
        struct sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int clientSocket = accept(_serverSocket, 
                (struct sockaddr*)&clientAddr, &clientSize);
        if(clientSocket == -1){
            _logger.log("Error accepting client connection");
            continue;
        }
        
        // TODO: add dynamic workers creation depending on load

        // add to queue
        {
            std::lock_guard<std::mutex> lock(_workerMutex);
            _logger.log("Add client socket to the queue");
            _pendingConnections.push_back(clientSocket);
        }
        cv.notify_one(); // notify waiting worker about new connection
    }
}

