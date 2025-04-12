#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./otoclient [ip] [port]\n";
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
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "❌ connection failed\n";
        close(sock);
        return 1;
    }

    std::cout << "✅ 연결 성공! 서버에 연결되었습니다.\n";

    // 🔽 클라이언트 자신의 IP와 포트 정보 가져오기
    sockaddr_in localAddr{};
    socklen_t addrLen = sizeof(localAddr);
    getsockname(sock, (struct sockaddr*)&localAddr, &addrLen);

    char localIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &localAddr.sin_addr, localIP, sizeof(localIP));
    int localPort = ntohs(localAddr.sin_port);

    std::ostringstream infoStream;
    infoStream << "CLIENT_INFO " << localIP << ":" << localPort;

    std::string infoMessage = infoStream.str();
    send(sock, infoMessage.c_str(), infoMessage.size(), 0);

    std::cout << "📤 내 정보 전송됨: " << infoMessage << std::endl;

    // 채팅 시작
    char buffer[1024];
    std::string message;
    while (true) {
        std::cout << "나: ";
        std::getline(std::cin, message);

        if (message == "exit") {
            std::cout << "🔌 연결 종료\n";
            break;
        }

        send(sock, message.c_str(), message.size(), 0);

        ssize_t bytesRead = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "서버: " << buffer << std::endl;
        } else if (bytesRead == 0) {
            std::cout << "서버가 연결을 종료했습니다.\n";
            break;
        } else {
            std::cerr << "❌ 메시지 수신 실패\n";
            break;
        }
    }

    close(sock);
    return 0;
}

