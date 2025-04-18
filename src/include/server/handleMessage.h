#ifndef HANDLEMESSAGE_H
#define HANDLEMESSAGE_H

#include "../utils/options.h"
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



class HandlerMessage {
public:
    static std::vector<int> clients;
    static std::mutex clientMutex;

    static void forwardMessage(const std::vector<int>& clients, const std::string& message, int senderSockfd); 
    static void handleRecv(int clientSockfd, std::vector<int>& clients, const ServerOptions& options);
};

#endif
