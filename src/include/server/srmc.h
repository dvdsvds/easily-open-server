#ifndef SRMC_H
#define SRMC_H

#include "../utils/options.h"
#include "handler.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void srmcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options);

#endif
