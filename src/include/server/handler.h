#ifndef HANDLEMESSAGE_H
#define HANDLEMESSAGE_H

#include "../utils/options.h"
#include <unordered_map>
#include <iostream>
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
    static std::vector<ClientInfo> clients;
    static std::mutex clientMutex;

    static void forwardMessage(const std::vector<ClientInfo>& clients, const std::string& message, int senderSockfd); 
    static void handleRecv(int clientSockfd, std::vector<ClientInfo>& clients, const ServerOptions& options);
    static std::string nickPrompt(int clientSockfd);
};

#endif
