#ifndef OTO_H
#define OTO_H

#include "../utils/options.h"
#include <sys/socket.h>

void ctcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options);

#endif
