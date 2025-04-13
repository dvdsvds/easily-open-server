#include "start_server.h"
#include "oto.h"
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void startServer(int port, const std::map<std::string, std::string>& options, const std::string& serverType) {
    int serverSockfd;
    struct sockaddr_in serverAddr, clientAddr;

    serverSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSockfd == -1) {
        std::cerr << "socket creation failed : " << std::system_error(errno, std::generic_category()).what() << std::endl;
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if(bind(serverSockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "socket binding failed : " << std::system_error(errno, std::generic_category()).what() << std::endl;
        close(serverSockfd);
        exit(EXIT_FAILURE);
    }

    if(listen(serverSockfd, 5) == -1) {
        std::cerr << "listening failed : " << std::system_error(errno, std::generic_category()).what() << std::endl;
        close(serverSockfd);
        exit(EXIT_FAILURE);
    }

    socklen_t clientlen;

    if(serverType == "oto") {
        std::cout << "oto server" << std::endl;
        clientlen = sizeof(clientAddr);
        otoServer(serverSockfd, clientAddr, clientlen);
    }
    else if(serverType == "mor") {
        std::cout << "multi one room server" << std::endl;
    }
    else if(serverType == "mr") {
        std::cout << "multi room server" << std::endl;
    }

    std::cout << "Client connected" << std::endl;

    close(serverSockfd);
}
