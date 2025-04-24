#include "handler.h"

std::vector<ClientInfo> Handler::clients;
std::mutex Handler::clientMutex;

void Handler::forwardMessage(const std::vector<ClientInfo>& clients, const std::string& message, int senderSockfd) {
    std::string senderNickname = "Unknown";
    for (const ClientInfo& client : clients) {
        if(client.sockfd == senderSockfd) {
            senderNickname = client.nickname;
            break;
        }
    }

    std::string fullMessage = senderNickname + " : " + message;

    for (const ClientInfo& client : clients) {
        if (client.sockfd != senderSockfd) {
            send(client.sockfd, fullMessage.c_str(), fullMessage.size(), 0);
        }
    }
}

void Handler::handleRecv(int clientSockfd, std::vector<ClientInfo>& clients, const ServerOptions& options) {
    char buffer[options.bufferSize];
    while(true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSockfd, buffer, sizeof(buffer) - 1, 0);

        if(bytesRead > 0) {
            buffer[bytesRead] = '\0';

            std::string nickname;
            for(const ClientInfo& client : clients) {
                if(client.sockfd == clientSockfd) {
                    nickname = client.nickname;
                    break;
                }
            }
            // std::cout << nickname <<  " : " << buffer << std::endl;

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
    clients.erase(std::remove_if(clients.begin(), clients.end(), 
                                 [clientSockfd](const ClientInfo& client) {
                                     return client.sockfd == clientSockfd;
                                 }), clients.end());
    close(clientSockfd);
}


std::string Handler::nickPrompt(int clientSockfd) {
    std::string promptMsg = "Enter your nickname: ";
    send(clientSockfd, promptMsg.c_str(), promptMsg.size(), 0);

    char buffer[64] = {0};
    int n = recv(clientSockfd, buffer, sizeof(buffer), 0);
    if (n <= 0) {
        std::cerr << "Failed to receive nickname. Disconnecting client." << std::endl;
        close(clientSockfd);
        return "";
    }

    std::string nickname(buffer, n);

    nickname.erase(nickname.find_last_not_of("\n\r") + 1);
    return nickname;  
}



