#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./client [ip] [port]\n";
        return 1;
    }

    const char* ip = argv[1];
    int port = std::stoi(argv[2]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "❌ socket creation failed\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0) {
        std::cerr << "❌ inet_pton failed\n";
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "❌ connected failed\n";
        close(sock);
        return 1;
    }

    std::cout << "✅ success!\n";

    close(sock);
    std::cout << "🔌 연결 종료됨\n";
    return 0;
}

