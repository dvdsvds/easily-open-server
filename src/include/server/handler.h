#ifndef HANDLEMESSAGE_H
#define HANDLEMESSAGE_H

#include "../utils/options.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <mutex>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <system_error>
#include <arpa/inet.h>
#include <cstring>



class Handler {
public:
    static std::vector<int> clients;
    static std::mutex clientMutex;
    static std::unordered_map<int, std::string> clientNickName;

    static void forwardMessage(const std::vector<int>& clients, const std::string& message, int senderSockfd); 
    static void handleRecv(int clientSockfd, std::vector<int>& clients, const ServerOptions& options);
    static std::string nickPrompt(int clientSockfd);
};

#endif
