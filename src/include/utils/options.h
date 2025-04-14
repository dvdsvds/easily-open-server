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
    int maxClient;
    int bufferSize;
    int maxReqSize;
    bool log = false;
    int timeout;
};

extern ServerOptions options;

void setServerOption(ServerOptions& options);

// void parseOptions(int argc, char* argv[], std::map<std::string, std::string>& options);
// void printOptions(const std::map<std::string, std::string>& options);
#endif
