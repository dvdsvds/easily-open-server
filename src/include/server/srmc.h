#ifndef SRMC_H
#define SRMC_H

#include "../utils/options.h"
#include "handleMessage.h"
#include <cstring>
#include <iostream>
#include <mutex>
#include <algorithm>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <system_error>
#include <arpa/inet.h>

void srmcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options);

#endif
