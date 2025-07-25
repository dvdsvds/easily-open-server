#ifndef SRMC_H
#define SRMC_H

#include "../utils/options.h"
#include <sys/socket.h>

void srmcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options);

#endif
