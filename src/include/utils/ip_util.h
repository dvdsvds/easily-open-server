#ifndef IP_UTIL_H
#define IP_UTIL_H

#include <map>
#include <iostream>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp); 
std::string getIP();

#endif
