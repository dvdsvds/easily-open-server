#include "ctc.h"
#include <thread>

void ctcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options) {
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
    
    std::cout << "Starting Client-to-Client server" << std::endl;
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
            std::cout << "Client IP : " << clientIP << ", port : " << ntohs(clientAddr.sin_port) << std::endl;
            
            {
                std::lock_guard<std::mutex> lock(HandlerMessage::clientMutex);
                HandlerMessage::clients.push_back(clientSockfd);
                clientCount++;
            }

            if(clientCount == 2 && HandlerMessage::clients.size() == 2) {
                std::cout << "2 clients connected." << std::endl;

                std::thread c1(HandlerMessage::handleRecv, HandlerMessage::clients[0], std::ref(HandlerMessage::clients), std::ref(options));
                std::thread c2(HandlerMessage::handleRecv, HandlerMessage::clients[1], std::ref(HandlerMessage::clients), std::ref(options));
                c1.detach();
                c2.detach();
            }
        }

        {
            std::lock_guard<std::mutex> lock(HandlerMessage::clientMutex);
            if(HandlerMessage::clients.size() == 0) {
                std::cout << "All clients disconnected. Shutting down server." << std::endl;
                break;
            }
        }

    }

    close(serverSockfd);
}
