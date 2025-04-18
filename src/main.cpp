#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "include/utils/options.h"
#include "include/server/start_server.h"


int main(int argc, char* argv[]) {
    ServerOptions options;
    setServerOption(options);

    if(options.serverType == "ctc") {
        startServer(options);
    }
    else if(options.serverType == "srmc") {
        startServer(options);
    }
    else if(options.serverType == "mr") {
        startServer(options);
    }

    return 0;
} 
