#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

void handleRecv(int sockfd) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';  // null-terminate the message
            std::cout << "\rOther Client: " << buffer << std::endl;
            std::cout << "> " << std::flush;
        }
        else if (bytesRead == 0) {
            std::cout << "Server disconnected." << std::endl;
            break;
        }
        else {
            std::cerr << "Error in receiving data." << std::endl;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [IP] [Port]" << std::endl;
        return 1;
    }

    const char* server_ip = argv[1];
    int server_port = std::stoi(argv[2]);

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        return 1;
    }

    // Server address structure
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port);

    // Convert IP address
    if (inet_pton(AF_INET, server_ip, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported" << std::endl;
        close(sockfd);
        return 1;
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed!" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Connected to server: " << server_ip << ":" << server_port << std::endl;

    // Start a thread for receiving messages from the server
    std::thread recvThread(handleRecv, sockfd);
    recvThread.detach();

    char buffer[BUFFER_SIZE];
    while (true) {
        std::cout << "> ";
        std::cin.getline(buffer, sizeof(buffer));

        if (strcmp(buffer, "exit") == 0) {
            std::cout << "Exiting the chat..." << std::endl;
            break;
        }

        // Send the message to the server
        ssize_t bytesSent = send(sockfd, buffer, strlen(buffer), 0);
        if (bytesSent == -1) {
            std::cerr << "Error in sending data." << std::endl;
            break;
        }
    }

    close(sockfd);
    return 0;
}

