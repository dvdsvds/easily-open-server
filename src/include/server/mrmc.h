#ifndef MRMC_H
#define MRMC_H

#include "../utils/options.h"
#include "handler.h"
#include <sys/socket.h>

void mrmcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options);

#endif
