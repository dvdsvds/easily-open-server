#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <csignal>

#define BUFFER_SIZE 1024

int sockfd = -1;
bool running = true;

void handleRecv(int sockfd) {
    char buffer[BUFFER_SIZE];
    while (running) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';  // null-terminate the message
            std::cout << "\r" << buffer << std::endl;
            std::cout << "> " << std::flush;
        }
        else if (bytesRead == 0) {
            std::cout << "\n[Disconnected by server]" << std::endl;
            running = false;
            break;
        }
        else {
            std::cerr << "\n[Error receiving data]" << std::endl;
            running = false;
            break;
        }
    }
}

void handleSigint(int) {
    std::cout << "\n[Interrupt received, shutting down...]" << std::endl;
    running = false;
    if (sockfd != -1) close(sockfd);
    exit(0);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handleSigint);

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [IP] [Port]" << std::endl;
        return 1;
    }

    const char* server_ip = argv[1];
    int server_port = std::stoi(argv[2]);

    // Connect to server
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address or not supported." << std::endl;
        close(sockfd);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed!" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Connected to " << server_ip << ":" << server_port << std::endl;

    // === 닉네임 입력 ===
    std::string nickname;
    std::getline(std::cin, nickname);

    if (nickname.empty()) {
        std::cerr << "Nickname cannot be empty!" << std::endl;
        close(sockfd);
        return 1;
    }

    // 서버에 닉네임 전송
    send(sockfd, nickname.c_str(), nickname.length(), 0);

    std::thread recvThread(handleRecv, sockfd);
    recvThread.detach();

    char buffer[BUFFER_SIZE];
    while (running) {
        std::cout << "> ";
        std::cin.getline(buffer, sizeof(buffer));

        if (strcmp(buffer, "exit") == 0) {
            std::cout << "Exiting the chat..." << std::endl;
            break;
        }

        ssize_t bytesSent = send(sockfd, buffer, strlen(buffer), 0);
        if (bytesSent == -1) {
            std::cerr << "Error sending data." << std::endl;
            break;
        }
    }

    running = false;
    close(sockfd);
    return 0;
}

