#include "ip_util.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

std::string getIP() {
    char hostname[256];
    if(gethostname(hostname, sizeof(hostname)) == 0) {
        struct hostent* host = gethostbyname(hostname);
        if(host && host->h_addr_list[0]) {
            return inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
        }
    }
    return "Unknown";
}
