#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "include/utils/ip_util.h"
#include "include/utils/options.h"
#include "include/server/start_server.h"

int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cerr << "Usage : " << argv[0] << " [server-type] [port] [options]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string serverType = argv[1];
    int port;

    try {
        port = std::stoi(argv[2]);
        if(port < 1 || port > 65535) throw std::out_of_range("Port must be between 1 and 65535");
    } catch (const std::exception& e) {
        std::cerr << "Invailed port number : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::map<std::string, std::string> options;
    parseOptions(argc, argv, options);
    printOptions(options);

    std::cout << "Starting server of type [" << serverType << "] on port [" << port << "]" << std::endl;
    std::cout << getIP() << std::endl;
    
    if (options.find("max-client") != options.end()) {
        int maxClient = std::stoi(options["max-client"]);
    }

    if (options.find("buffer-size") != options.end()) {
        int bufferSize = std::stoi(options["buffer-size"]);
    }

    if(serverType == "oto") {
        startServer(port, options, "oto");
    }
    else if(serverType == "mor") {
        startServer(port, options, "mor");
    }
    else if(serverType == "mr") {
        startServer(port, options, "mr");
    }

    return 0;
} 
