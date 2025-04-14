#include "options.h"

void setServerOption(ServerOptions& options) {
    std::cout << "Enter server type (default: oto) : ";
    std::string type;
    std::getline(std::cin, type);
    if(!type.empty()) {
        options.serverType = type;
    }
    else {
        options.serverType = "oto";
    }

    // SET PORT
    std::cout << "Enter port (default : 8080) : ";
    std::string portInput;
    std::getline(std::cin, portInput);
    if(!portInput.empty()) {
        try {
            options.port = std::stoi(portInput);
        }
        catch(const std::exception e) {
            std::cerr << "Invalid port, using default 8080." << std::endl;
            options.port = 8080;
        }
    }
    else {
        options.port = 8080;
    }

    // SET MAX CLIENT
    std::cout << "Enter Max Client (default : 2) : ";
    std::string maxclientInput;
    std::getline(std::cin, maxclientInput);
    if(!maxclientInput.empty()) {
        try {
            options.maxClient = std::stoi(maxclientInput);
        }
        catch(const std::exception e) {
            std::cerr << "Invalid value, using default 2." << std::endl;
            options.maxClient = 2;
        }
    }
    else {
        options.maxClient = 2;
    }

    // SET BUFFER SIZE
    std::cout << "Enter Buffer Size (default : 1024) : ";
    std::string buffersizeInput;
    std::getline(std::cin, buffersizeInput);
    if(!buffersizeInput.empty()) {
        try {
            options.bufferSize = std::stoi(buffersizeInput);
        }
        catch(const std::exception e) {
            std::cerr << "Invalid value, using default 1024." << std::endl;
            options.bufferSize = 1024;
        }
    }
    else {
        options.bufferSize = 1024;
    }

    // SET MAX REQUEST SIZE
    // std::cout << "Enter Max Request Size (default : 8192) : ";
    // std::string maxreqsizeInput;
    // std::getline(std::cin, maxreqsizeInput);
    // if(!maxreqsizeInput.empty()) {
    //     try {
    //         options.maxReqSize = std::stoi(maxreqsizeInput);
    //     }
    //     catch(const std::exception e) {
    //         std::cerr << "Invalid value, using default 8192." << std::endl;
    //         options.maxReqSize = 8192;
    //     }
    // }
    // else {
    //     options.maxReqSize = 8192;
    // }

    // SET LOGGING
    std::cout << "Enter LOG (default : false -> True - 1 | False - 0) : ";
    std::string logInput;
    std::getline(std::cin, logInput);

    if(!logInput.empty()) {
        try {
            int logValue = std::stoi(logInput);
            if(logValue == 1) {
                options.log = true;
            }
            else if(logValue == 0) {
                options.log = false;
            }
        }
        catch(const std::exception e) {
            std::cerr << "Invalid value, using default false." << std::endl;
            options.log = false;
        }
    }
    else {
        options.log = false;
    }

    // SET TIMEOUT
    std::cout << "Enter Time Out (default 60) : ";
    std::string timeoutInput;
    std::getline(std::cin, timeoutInput);
    if(!timeoutInput.empty()) {
        try {
            options.timeout = std::stoi(timeoutInput);
        }
        catch(const std::exception e) {
            std::cerr << "Invalid value, using default 60." << std::endl;
            options.timeout = 60;
        }
    }
    else {
        options.timeout = 60;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
