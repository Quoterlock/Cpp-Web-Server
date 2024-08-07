#include"httpReq.h"
#include"httpRes.h"
#include"router.h"
#include"httpParser.h"

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

const int PORT = 8080;
const char* ADDRESS = "127.0.0.1";
const int MAX_CLIENTS_COUNT = 10;
int serverSocket;

void slog(std::string msg){
    std::cout << "[Server]: " << msg << "\n"; 
}

void exitSignalHandler(int signal) {
    close(serverSocket);
    exit(EXIT_SUCCESS);
}

void handleClient(int clientSocket) {
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    // receive a client message
    memset(buffer, 0, BUFFER_SIZE);
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if(bytesReceived == -1){
        std::cerr << "Error receiving message!\n";
        exit(EXIT_FAILURE);
    }
    
    // process message
    auto request = decodeHttp(buffer);
    auto response = route(request);
    std::string responseStr = encodeResponse(response);
    
    // send a response
    int bytesSent = send(clientSocket, responseStr.c_str(), strlen(responseStr.c_str()), 0);            
    if(bytesSent == -1) {
        std::cerr << "Error sending response\n";
        exit(EXIT_FAILURE);
    }
}

void initServerSocket(){
    // create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1) {
        std::cerr << "Error creating socket!\n";
        exit(EXIT_FAILURE);
    }
    // config address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // use IPv4
    serverAddr.sin_port = htons(PORT);
    if(inet_aton(ADDRESS, &serverAddr.sin_addr) <= 0){
        std::cerr << "Error converting an address!\n";
        exit(EXIT_FAILURE);
    }
    // bind socket
    if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
        std::cerr << "Error binding server socket\n";
        exit(EXIT_FAILURE);
    }
    // set listener
    if(listen(serverSocket, MAX_CLIENTS_COUNT) == -1) {
        std::cerr << "Error listening!\n";
    } else {
        std::cout << "Listen port : " << PORT << "\n"; 
    }
}

int main(){
    // handle sever closing
    signal(SIGINT, exitSignalHandler);

    initServerSocket();

    // process clients
    while(true){
        std::cout << "Waiting for client...\n";
        struct sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientSize);
        
        auto pid = fork();
        if(pid == 0) {
            // child
            std::cout << "Client connected\n";
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

    close(serverSocket);
    return 0;
}
