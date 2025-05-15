#include "start_server.h"
#include "ctc.h"
#include "srmc.h"
// #include "mrmc.h"
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void startServer(const ServerOptions& options) {
    int serverSockfd;
    struct sockaddr_in serverAddr, clientAddr;

    serverSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSockfd == -1) {
        std::cerr << "socket creation failed : " << std::system_error(errno, std::generic_category()).what() << std::endl;
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(options.port);

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

    if(options.serverType == "ctc") {
        clientlen = sizeof(clientAddr);
        ctcServer(serverSockfd, clientAddr, clientlen, options);
    }
    else if(options.serverType == "srmc") {
        clientlen = sizeof(clientAddr);
        srmcServer(serverSockfd, clientAddr, clientlen, options);
    }
    // else if(options.serverType == "mrmc") {
    //     clientlen = sizeof(clientAddr);
    //     mrmcServer(serverSockfd, clientAddr, clientlen, options);
    // }

    std::cout << "Client connected" << std::endl;

    close(serverSockfd);
}
