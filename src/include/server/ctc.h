#ifndef OTO_H
#define OTO_H

#include "../utils/options.h"
#include "handler.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>


void ctcServer(int serverSockfd, struct sockaddr_in clientAddr, socklen_t clientlen, const ServerOptions& options);

#endif
