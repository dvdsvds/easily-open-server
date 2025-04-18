#include "mrmc.h"
#include "handler.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <mutex>
#include <netinet/in.h>
#include <sys/socket.h>
#include <system_error>

void mrmcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options) {
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

    std::cout << "Starting Multi Room - Multi Client" << std::endl;

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

            std::thread t(Handler::handleRecv, clientSockfd, std::ref(Handler::clients), std::ref(options));
            t.detach();
        }

        {
            std::lock_guard<std::mutex> lock(Handler::clientMutex);
            if(Handler::clients.size() == 0) {
                std::cout << "All clients disconnected. Shutting down server." << std::endl;
                break;
            }
        }
    }
}
