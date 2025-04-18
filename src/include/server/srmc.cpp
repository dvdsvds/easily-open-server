#include "srmc.h"

void srmcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options) {
    char clientIP[INET_ADDRSTRLEN]; 
    inet_ntop(AF_INET, &(clientAddr), clientIP, INET_ADDRSTRLEN);

    std::cout << "starting One Room - Multi Client server" << std::endl;
    std::cout << options.serverType << std::endl;
    std::cout << options.maxClient << std::endl;
    std::cout << options.bufferSize << std::endl;

    int clientCount = 0;

    while(true) {
        if(clientCount < options.maxClient) {
            int clientSockfd = accept(serverSockfd, (struct sockaddr*)&clientAddr, &clientlen); 
            if(clientSockfd == -1) {
                std::cerr << "Accept failed : " << std::system_error(errno, std::generic_category()).what() << std::endl;
                close(serverSockfd);
                exit(EXIT_FAILURE);
            }

            inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
            std::cout << "Client IP :" << clientIP << ", port : " << ntohs(clientAddr.sin_port) << std::endl;

            { 
                std::lock_guard<std::mutex> lock(HandlerMessage::clientMutex);
                HandlerMessage::clients.push_back(clientSockfd);
                clientCount++;
            }

            std::thread t(HandlerMessage::handleRecv, clientSockfd, std::ref(HandlerMessage::clients), std::ref(options));
            t.detach();
        }

        {
            std::lock_guard<std::mutex> lock(HandlerMessage::clientMutex);
            if(HandlerMessage::clients.size() == 0) {
                std::cout << "All clients disconnected. Shutting down server." << std::endl;
                break;
            }
        }

    }
}

