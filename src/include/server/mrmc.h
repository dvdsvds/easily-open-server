#ifndef MRMC_H
#define MRMC_H

#include "../utils/options.h"
#include "handleMessage.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>

void mrmcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options);

#endif
