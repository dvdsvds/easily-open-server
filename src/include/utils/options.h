#ifndef OPTIONS_H
#define OPTIONS_H

#include <list>
#include <map>
#include <iostream>
#include <cstdlib>
#include <ios>
#include <limits>
#include <atomic>

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

struct RoomInfo {
    int roomId;
    std::string roomName;
    std::list<ClientInfo*> users;
};

extern std::atomic<bool> hasClientConnected;
extern ServerOptions options;
extern ClientInfo info;
extern RoomInfo rinfo;


void setServerOption(ServerOptions& options);

#endif
