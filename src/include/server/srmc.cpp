#include "srmc.h"

std::vector<int> clients;
std::mutex clientMutex;

ServerOptions optios;

void handleRecv(int clientSockfd, std::vector<int>& clients, const ServerOptions& options) {

}
void fowardMessage(const std::vector<int>& clients, const std::string& message, int senderSockfd) {

}
void srmcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options) {
    char clientIP[INET_ADDRSTRLEN]; 
    inet_ntop(AF_INET, &(clientAddr), clientIP, INET_ADDRSTRLEN);

    std::cout << "starting One Room - Multi Client server" << std::endl;

    int clientCount = 0;

    while(true) {
        if(clientCount == options.maxClient) {
            int clientSockfd = accept(serverSockfd, (struct sockaddr*)&clientAddr, &clientlen); 
            if(clientSockfd == -1) {
                std::cerr << "Accept failed : " << std::system_error(errno, std::generic_category()).what() << std::endl;
                close(serverSockfd);
                exit(EXIT_FAILURE);
            }

            inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
            std::cout << "Client IP :" << clientIP << ", port : " << ntohs(clientAddr.sin_port) << std::endl;

            { 
                std::lock_guard<std::mutex> lock(clientMutex);
                clients.push_back(clientSockfd);
                clientCount++;
            }
        }
    }
}

