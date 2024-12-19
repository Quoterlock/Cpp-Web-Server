#include "server.h"
#include "logger.h"
#include "httpParser.h"
#include "router.h"

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<sys/un.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
#include<iostream>
#include<signal.h>

HttpServer::HttpServer(Logger logger, Router router){
    _logger = logger; 
    _port = 8080;
    _address = "127.0.0.1";
    _maxClientsCount = 10;
    _router = router;
}

void HttpServer::setPort(int port){
    _port = port;
}

void HttpServer::setAddress(std::string address){
    _address = address;
}


void HttpServer::run(){
    initServerSocket();

    // process clients
    while(true){
        _logger.log("Waiting for client...");
        struct sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientSize);
        // TODO: Rework for threads pool
        auto pid = fork();
        if(pid == 0) {
            // child
            _logger.log("Client connected");
            handleClient(clientSocket);
            close(clientSocket);
        } else if(pid > 0) {
            // parent
            close(clientSocket); 
        } else {
            // error
            std::cerr << "Failed to fork.\n";
            exit(EXIT_FAILURE);
        }
    }

    close(_serverSocket);
}

void HttpServer::handleClient(int clientSocket) {
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    // receive a client message
    memset(buffer, 0, BUFFER_SIZE);
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if(bytesReceived == -1){
        _logger.log("Error receiving message!");
        exit(EXIT_FAILURE);
    }
    
    // process message
    auto request = decodeHttp(buffer);
    auto response = _router.route(request);
    std::string responseStr = encodeResponse(response);
    
    // send a response
    int bytesSent = send(clientSocket, responseStr.c_str(), strlen(responseStr.c_str()), 0);            
    if(bytesSent == -1) {
        _logger.log("Error sending response");
        exit(EXIT_FAILURE);
    }
}

void HttpServer::initServerSocket(){
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
    serverAddr.sin_port = htons(_port);
    if(inet_aton(_address.c_str(), &serverAddr.sin_addr) <= 0){
        _logger.log("Error converting an address!");
        exit(EXIT_FAILURE);
    }
    // bind socket
    if(bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
        _logger.log("Error binding server socket");
        exit(EXIT_FAILURE);
    }
    // set listener
    if(listen(_serverSocket, _maxClientsCount) == -1) {
        _logger.log("Error listening!");
    } else {
        _logger.log("Listening port " + std::to_string(_port)); 
    }
}
