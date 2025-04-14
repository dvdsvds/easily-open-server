#ifndef OTO_H
#define OTO_H

#include <cstring>
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
#include "../utils/options.h"

void otoServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options);
void handleRecv(int clientSockfd, std::vector<int>& clients, const ServerOptions& options);
void handleSend(int clientSockfd);
void forwardMessage(const std::vector<int>& clients, const std::string& message, int senderSockfd);

#endif
