#include "ctc.h"
#include "handler.h"
#include <arpa/inet.h>
#include <thread>

void ctcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options) {
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
    
    std::cout << "Starting Client-to-Client server" << std::endl;

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
                std::lock_guard<std::mutex> lock(Handler::clientMutex);
                Handler::clients.push_back(clientSockfd);
                clientCount++;
            }

            if(clientCount == 2 && Handler::clients.size() == 2) {
                std::cout << "2 clients connected." << std::endl;

                std::thread c1(Handler::handleRecv, Handler::clients[0], std::ref(Handler::clients), std::ref(options));
                std::thread c2(Handler::handleRecv, Handler::clients[1], std::ref(Handler::clients), std::ref(options));
                c1.detach();
                c2.detach();
            }
        }

        {
            std::lock_guard<std::mutex> lock(Handler::clientMutex);
            if(Handler::clients.size() == 0) {
                std::cout << "All clients disconnected. Shutting down server." << std::endl;
                break;
            }
        }

    }

    close(serverSockfd);
}
