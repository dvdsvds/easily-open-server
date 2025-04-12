#ifndef OPTIONS_H
#define OPTIONS_H

#include <map>
#include <iostream>
#include <cstdlib>

void parseOptions(int argc, char* argv[], std::map<std::string, std::string>& options);
void printOptions(const std::map<std::string, std::string>& options);
#endif
