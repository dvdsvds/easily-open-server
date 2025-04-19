#include "handler.h"

std::vector<int> Handler::clients;
std::mutex Handler::clientMutex;

void Handler::forwardMessage(const std::vector<int>& clients, const std::string& message, int senderSockfd) {
    for (int clientSockfd : clients) {
        if (clientSockfd != senderSockfd) {
            send(clientSockfd, message.c_str(), message.size(), 0);
        }
    }
}


void Handler::handleRecv(int clientSockfd, std::vector<int>& clients, const ServerOptions& options) {
    char buffer[options.bufferSize];
    while(true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSockfd, buffer, sizeof(buffer) - 1, 0);

        if(bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "CLIENT : " << buffer << std::endl;

            std::lock_guard<std::mutex> lock(Handler::clientMutex);
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
