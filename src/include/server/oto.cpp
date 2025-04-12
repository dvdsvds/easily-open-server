#include "oto.h"

void handleRecv(int clientSockfd) {

}
void otoServer(int serverSockfd, int clientSockfd, struct sockaddr_in clientAddr, socklen_t clientlen) {
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
    
    std::cout << "Starting 1-to-1 server" << std::endl;

    clientSockfd = accept(serverSockfd, (struct sockaddr*)&clientAddr, &clientlen);
    std::cout << "Client IP : " << clientIP << ", port : " << ntohs(clientAddr.sin_port) << std::endl;
    if(clientSockfd == -1) {
        std::cerr << "Accept failed : " << std::system_error(errno, std::generic_category()).what() << std::endl;
        close(serverSockfd);
        exit(EXIT_FAILURE);
    }

    char buffer[1024] = {0};
    ssize_t bytesRead;

    while(true) {
        memset(buffer, 0, sizeof(buffer));
        bytesRead = recv(clientSockfd, buffer, sizeof(buffer) - 1, 0);

        if(bytesRead == -1) {
            std::cerr << "Error in receving data from client." << std::endl;
            break;
        }
        else if(bytesRead == 0) {
            std::cout << "Client disconnected." << std::endl; 
            break;
        }

        std::cout << "CLIENT : " << buffer << std::endl;

        if(strcmp(buffer, "exit") == 0) {
            std::cout << "Client requested to close the connection." << std::endl;
            break;
        }

        std::string response = "SERVER : " + std::string(buffer);
        ssize_t bytesSent = send(clientSockfd, response.c_str(), response.length(), 0);
        if(bytesSent == -1) {
            std::cerr << "Error in sending data to client." << std::endl;
            break;
        }
    } 

    close(clientSockfd);
    close(serverSockfd);
}
