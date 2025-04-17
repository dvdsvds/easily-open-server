#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "include/utils/ip_util.h"
#include "include/utils/options.h"
#include "include/server/start_server.h"


int main(int argc, char* argv[]) {
    ServerOptions options;
    setServerOption(options);

    // std::cout << "Starting server of type [" << options.serverType << "] on port [" << options.port << "]" << std::endl;
    // std::cout << "IP : " << getIP() << std::endl;
    // std::cout << "PORT : " << options.port << std::endl;
    // std::cout << "Type : " << options.serverType << std::endl;
    // std::cout << "Max Client : " << options.maxClient << std::endl;
    // std::cout << "Max Request Size : " << options.serverType << std::endl;
    // std::cout << "LOG : " << options.log << std::endl;
    // std::cout << "TIMEOUT : " << options.timeout << std::endl;
    
    if(options.serverType == "ctc") {
        startServer(options);
    }
    else if(options.serverType == "mor") {
        startServer(options);
    }
    else if(options.serverType == "mr") {
        startServer(options);
    }

    return 0;
} 
