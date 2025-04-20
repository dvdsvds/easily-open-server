#include "handler.h"

std::vector<ClientInfo> Handler::clients;
std::mutex Handler::clientMutex;

void Handler::forwardMessage(const std::vector<ClientInfo>& clients, const std::string& message, int senderSockfd) {
    for (const ClientInfo& client : clients) {
        if (client.sockfd != senderSockfd) {
            send(client.sockfd, message.c_str(), message.size(), 0);
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
            std::cout << nickname <<  " : " << buffer << std::endl;

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

// void Handler::nickPrompt(int sockfd) {
//     char buffer[128] = {0};
//     int n = recv(sockfd, buffer, sizeof(buffer), 0);
//     if(n > 0) {
//         std::cout << std::string(buffer, n);
//     }
//
//     std::string nickname;
//     std::getline(std::cin, nickname);
//     send(sockfd, nickname.c_str(), nickname.size(), 0);
// }

// std::string Handler::nickPrompt(int clientSockfd) {
//     std::string promptMsg = "Enter your nickname: ";
//     send(clientSockfd, promptMsg.c_str(), promptMsg.size(), 0);
//
//     char buffer[64] = {0};
//     int n = recv(clientSockfd, buffer, sizeof(buffer), 0);
//     if (n <= 0) {
//         std::cerr << "Failed to receive nickname. Disconnecting client." << std::endl;
//         close(clientSockfd);
//         return "";
//     }
//
//     std::string nickname(buffer, n);
//     return nickname;  // 이제 std::string을 반환합니다
// }


std::string Handler::nickPrompt(int sockfd) {
    char buffer[128] = {0};  

    int n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n > 0) {
        std::string nickname(buffer, n);  
        std::cout << "Received nickname: " << nickname << std::endl;
        return nickname;  
    } else {
        std::cerr << "Error receiving nickname from client." << std::endl;
        return "";  
    }
}

