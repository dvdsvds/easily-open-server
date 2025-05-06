#include "options.h"
#include <algorithm>
#include <set>

ClientInfo info;
RoomInfo rinfo;
std::atomic<bool> hasClientConnected;

const std::string DEFAULT_SERVER_TYPE = "ctc";
const std::set<std::string> TYPES = {"ctc", "srmc", "mrmc"};

const int DEFAULT_MAX_ROOM = 2; const int DEFAULT_FIXED_ROOM = 1;
const int DEFAULT_MAX_CLIENT = 2; const int DEFAULT_FIXED_CLIENT = 1;

const int DEFAULT_BUFFER_SIZE = 2048;
const int MIN_BUFFER_SIZE = 1024;
const int MAX_BUFFER_SIZE = 67108864;

const int DEFAULT_TIMEOUT = 120;
const int MIN_TIMEOUT = 1;
const int MAX_TIMEOUT = 600;

const int DEFAULT_PORT = 8080;
const int MIN_PORT = 1024;
const int MAX_PORT = 65535;

const std::set<int> RESERVED_PORTS = {
    20, 21, 22, 23, 25, 53, 80, 
    110, 143, 161, 389, 443, 465, 587, 993, 995, 
    1433, 1521, 3306, 5432, 6379, 
    11211, 27017
};

void toLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void setServerOption(ServerOptions& options) {
    std::cout << "Enter server type (default : "<<  DEFAULT_SERVER_TYPE << ") : ";
    std::string typeInput;
    std::getline(std::cin, typeInput);
    if(!typeInput.empty()) {
        toLower(typeInput);
        if(TYPES.count(typeInput)) {
            options.serverType = typeInput;
        }
        else {
            std::cerr << "Invalid Server type '" << typeInput << "', using default " << DEFAULT_SERVER_TYPE << std::endl;
            options.serverType = DEFAULT_SERVER_TYPE;
        }
    }
    else {
        options.serverType = DEFAULT_SERVER_TYPE;
    }

    // SET PORT
    std::cout << "Enter port (default : " << DEFAULT_PORT << ") : ";
    std::string portInput;
    std::getline(std::cin, portInput);
    if(!portInput.empty()) {
        try {
            int val = std::stoi(portInput);
            if(val < MIN_PORT || val > MAX_PORT || RESERVED_PORTS.count(val)) {
                std::cerr << "Invalid or reserved port, using default port (" << DEFAULT_PORT << ")" << std::endl;
                options.port = DEFAULT_PORT;
            }
            else {
                options.port = val;
            }

        }
        catch(const std::exception& e) {
            std::cerr << "Invalid port, using default port(" <<  DEFAULT_PORT << ")" << std::endl;
            options.port = DEFAULT_PORT;
        }
    }
    else {
        options.port = DEFAULT_PORT;
    }

    if(options.serverType != "ctc" && options.serverType != "srmc") {
        std::cout << "Enter Max Room (default : " << DEFAULT_MAX_ROOM << ") : ";
        std::string maxRoomInput;
        std::getline(std::cin, maxRoomInput);
        if(!maxRoomInput.empty()) {
            try {
                int val = std::stoi(maxRoomInput);
                if(val > 0 && val <= 1000) {
                    options.maxRoom = val;
                }
                else {
                    std::cerr << "Max room must be within the valid range of 1 to 1000, using default " << DEFAULT_MAX_ROOM << std::endl;
                    options.maxRoom = DEFAULT_MAX_ROOM;
                }
            }
            catch(const std::exception& e) {
                std::cerr << "Invalid value, using default " << DEFAULT_MAX_ROOM << std::endl;
                options.maxRoom = DEFAULT_MAX_ROOM;
            }
        }
        else {
            options.maxRoom = 2;
        }
    }
    else {
        std::cout << "ctc and srmc server's maxRoom option is fixed to " << DEFAULT_FIXED_ROOM << std::endl;
        options.maxRoom = DEFAULT_FIXED_ROOM;
    }

    // SET MAX CLIENT
    if(options.serverType != "ctc") {
        std::cout << "Enter Max client (default : " << DEFAULT_MAX_CLIENT << ") : ";
        std::string maxclientInput;
        std::getline(std::cin, maxclientInput);
        if(!maxclientInput.empty()) {
            try {
                int val = std::stoi(maxclientInput);
                if(val < 2) {
                    std::cerr << "Max client value must be at least 2, using default " << DEFAULT_MAX_CLIENT << std::endl;
                    options.maxClient = DEFAULT_MAX_CLIENT;
                }
                else {
                    options.maxClient = val;
                }
            }
            catch(const std::exception& e) {
                std::cerr << "Invalid value, using default " << DEFAULT_MAX_CLIENT  << std::endl;
                options.maxClient = DEFAULT_MAX_CLIENT;
            }
        }
        else {
            options.maxClient = DEFAULT_MAX_CLIENT;
        }
    }
    else {
        std::cout << "ctc server's maxClient option is fixed to " << DEFAULT_MAX_CLIENT << std::endl;
        options.maxClient = DEFAULT_FIXED_CLIENT;
    }

    // SET BUFFER SIZE
    std::cout << "Enter Buffer size (default : " << DEFAULT_BUFFER_SIZE << ") : ";
    std::string buffersizeInput;
    std::getline(std::cin, buffersizeInput);
    if(!buffersizeInput.empty()) {
        try {
            int val = std::stoi(buffersizeInput);
            if(val < MIN_BUFFER_SIZE || val > MAX_BUFFER_SIZE) {
                std::cerr << "Cannot set the buffer size to a value less than " << MIN_BUFFER_SIZE << "or greater than " << MAX_BUFFER_SIZE  << std::endl;
                std::cerr << "Buffer Size option is set to the default value of " << DEFAULT_BUFFER_SIZE << std::endl;
                options.bufferSize = DEFAULT_BUFFER_SIZE;
            }
            else {
                options.bufferSize = val;
            }

        }
        catch(const std::exception& e) {
            std::cerr << "Invalid value, using default " << DEFAULT_BUFFER_SIZE << std::endl;
            options.bufferSize = DEFAULT_BUFFER_SIZE;
        }
    }
    else {
        options.bufferSize = DEFAULT_BUFFER_SIZE;
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
    std::cout << "Enter LOG (default : false) : ";
    std::string logInput;
    std::getline(std::cin, logInput);

    if(!logInput.empty()) {
        toLower(logInput);
        if(logInput == "true") {
            options.log = true;
        }
        else if(logInput == "false") {
            options.log = false;
        }
        else {
            std::cerr << "Invalid value, using default false" << std::endl;
            options.log = false;
        }
    }

    // SET TIMEOUT
    std::cout << "Enter Time out (default : " << DEFAULT_TIMEOUT << ") : ";
    std::string timeoutInput;
    std::getline(std::cin, timeoutInput);
    if(!timeoutInput.empty()) {
        try {
            int val = std::stoi(timeoutInput);
            if(val < MIN_TIMEOUT || val > MAX_TIMEOUT) {
                std::cerr << "Timeout must be between " << MIN_TIMEOUT << " and " << MAX_TIMEOUT << " seconds, using default " << DEFAULT_TIMEOUT << std::endl;
                options.timeout = DEFAULT_TIMEOUT;
            }
        }
        catch(const std::exception& e) {
            std::cerr << "Invalid value, using default " << DEFAULT_TIMEOUT << std::endl;
            options.timeout = DEFAULT_TIMEOUT;
        }
    }
    else {
        options.timeout = DEFAULT_TIMEOUT;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
