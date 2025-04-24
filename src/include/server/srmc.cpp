#include "srmc.h"
#include "handler.h"
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>

void srmcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options) {
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr), clientIP, INET_ADDRSTRLEN);

    std::cout << "starting One Room - Multi Client server" << std::endl;

    std::thread([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::lock_guard<std::mutex> lock(Handler::clientMutex);
            if (hasClientConnected && Handler::clients.empty()) {
                std::cout << "All clients disconnected. Shutting down server." << std::endl;
                std::exit(0);
            }
        }
    }).detach();

    while (true) {
        if (Handler::clients.size() < options.maxClient) {
            int clientSockfd = accept(serverSockfd, (struct sockaddr*)&clientAddr, &clientlen);
            if (clientSockfd == -1) {
                std::cerr << "Accept failed: " << std::system_error(errno, std::generic_category()).what() << std::endl;
                continue;
            }

            inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
            int clientPort = ntohs(clientAddr.sin_port);

            std::thread([clientSockfd, clientAddr, clientPort, options]() {
                std::string nickname = Handler::nickPrompt(clientSockfd);
                if (nickname.empty()) {
                    std::cerr << "Client did not provide nickname, skipping connection." << std::endl;
                    close(clientSockfd);
                    return;
                }

                info.sockfd = clientSockfd;
                info.port = clientPort;
                info.nickname = nickname;

                std::cout << "Client IP: " << inet_ntoa(clientAddr.sin_addr) << ", port: " << clientPort << std::endl;

                {
                    std::lock_guard<std::mutex> lock(Handler::clientMutex);
                    Handler::clients.push_back(info);
                    hasClientConnected = true;
                }

                std::thread t(Handler::handleRecv, clientSockfd, std::ref(Handler::clients), std::ref(options));
                t.detach();
            }).detach();
        }
    }
}
