#include "oto.h"
#include <thread>

std::vector<int> clients;
std::mutex clientMutex;


ServerOptions options;

void forwardMessage(const std::vector<int>& clients, const std::string& message, int senderSockfd) {
    for (int clientSockfd : clients) {
        if (clientSockfd != senderSockfd) {
            send(clientSockfd, message.c_str(), message.size(), 0);
        }
    }
}


void handleRecv(int clientSockfd, std::vector<int>& clients, const ServerOptions& options) {
    char buffer[options.bufferSize];
    while(true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSockfd, buffer, sizeof(buffer) - 1, 0);

        if(bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "CLIENT : " << buffer << std::endl;

            std::lock_guard<std::mutex> lock(clientMutex);
            forwardMessage(clients, buffer, clientSockfd);
        }
        else {
            if(bytesRead == 0) {
                std::cout << "Client disconnected." << std::endl; 
                break;
            }
            else {
                std::cerr << "Error in receving data from client." << std::endl;
                break;
            }

        }

        if(strcmp(buffer, "exit") == 0) {
            std::cout << "Client requested to close the connection." << std::endl;
            break;
        }
        
    }
    std::lock_guard<std::mutex> lock(clientMutex);
    clients.erase(std::remove(clients.begin(), clients.end(), clientSockfd), clients.end());
    close(clientSockfd);
}

void otoServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options) {
    std::cout << "servertype : " << options.serverType <<std::endl;
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
    
    std::cout << "Starting 1-to-1 server" << std::endl;

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
                std::lock_guard<std::mutex> lock(clientMutex);
                clients.push_back(clientSockfd);
                clientCount++;
            }

            if(clients.size() == 2) {
                std::cout << "2 clients connected." << std::endl;

                std::thread c1(handleRecv, clients[0], std::ref(clients), std::ref(options));
                std::thread c2(handleRecv, clients[1], std::ref(clients), std::ref(options));
                c1.detach();
                c2.detach();
            }
        }

        {
            std::lock_guard<std::mutex> lock(clientMutex);
            if(clients.size() == 0) {
                std::cout << "All clients disconnected. Shutting down server." << std::endl;
                break;
            }
        }

    }

    close(serverSockfd);
}
