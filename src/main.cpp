#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char* argv[]) {
    int serverSockfd, clientSockfd;
    struct sockaddr_in serverAddr, clientAddr;
    int port;

    if(argc < 2) {
        std::cerr << "Usage : "  << argv[0] << " <port>" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        port = std::stoi(argv[1]);
        if(port < 1 || port > 65535) throw std::out_of_range("Port must be between 1 and 65535");
    } catch (const std::exception& e) {
        std::cerr << "Invailed port number : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

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

    std::cout << "Server is running on port : " << port << std::endl;
    close(serverSockfd);
    return 0;
} 
