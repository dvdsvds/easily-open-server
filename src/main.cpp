#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "include/utils/ip_util.h"
#include "include/utils/options.h"

int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cerr << "Usage : " << argv[0] << " [port] [server-type] [options]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string serverType = argv[1];
    int port;

    try {
        port = std::stoi(argv[2]);
        if(port < 1 || port > 65535) throw std::out_of_range("Port must be between 1 and 65535");
    } catch (const std::exception& e) {
        std::cerr << "Invailed port number : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::map<std::string, std::string> options;
    parseOptions(argc, argv, options);
    printOptions(options);

    std::cout << "Starting server of type [" << serverType << "] on port [" << port << "]" << std::endl;
    std::cout << getIP() << std::endl;
    
    if (options.find("max-client") != options.end()) {
        int maxClient = std::stoi(options["max-client"]);
    }

    if (options.find("buffer-size") != options.end()) {
        int bufferSize = std::stoi(options["buffer-size"]);
    }

    int serverSockfd, clientSockfd;
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

    socklen_t clientlen = sizeof(clientAddr);
    if(accept(serverSockfd, (struct sockaddr*)&clientAddr, &clientlen) == -1) {
        std::cerr << "accept failed : " << std::system_error(errno, std::generic_category()).what() << std::endl;
        close(serverSockfd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Client connected" << std::endl;

    close(serverSockfd);
    return 0;
} 
