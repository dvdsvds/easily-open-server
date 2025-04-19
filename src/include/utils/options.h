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

extern ServerOptions options;

void setServerOption(ServerOptions& options);

#endif
