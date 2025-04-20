#ifndef OPTIONS_H
#define OPTIONS_H

#include <map>
#include <iostream>
#include <cstdlib>
#include <ios>
#include <limits>

struct ServerOptions {
    std::string serverType;
    int port;
    int maxRoom;
    int maxClient;
    int bufferSize;
    int maxReqSize;
    bool log = false;
    int timeout;
};

struct ClientInfo {
    int sockfd;
    std::string ip;
    int port;
    std::string nickname;

    bool operator==(int sockfd) const {
        return this->sockfd == sockfd;
    }
};

extern ServerOptions options;
extern ClientInfo info;

void setServerOption(ServerOptions& options);

#endif
