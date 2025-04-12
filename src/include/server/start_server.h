#ifndef START_SERVER_H
#define START_SERVER_H

#include <map>
#include <iostream>

void startServer(int port, const std::map<std::string, std::string>& options, const std::string& serverType);
#endif
