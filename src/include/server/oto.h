#ifndef OTO_H
#define OTO_H

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <system_error>
#include <arpa/inet.h>

void otoServer(int serverSockfd, int clientSockfd, struct sockaddr_in clientAddr, socklen_t clientlen);
#endif
