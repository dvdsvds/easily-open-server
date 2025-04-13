// #include "options.h"
//
// void parseOptions(int argc, char* argv[], std::map<std::string, std::string>& options) {
//     for (int i = 2; i < argc; i++) {
//         std::string arg = argv[i];
//
//         if (arg.find("--") == 0) {
//             size_t equalPos = arg.find("=");
//             if(equalPos == std::string::npos) {
//                 std::cerr << "Invalid option format : " << arg << std::endl;
//                 exit(EXIT_FAILURE);
//             }
//
//             std::string option = arg.substr(2, equalPos - 2);
//             std::string value = arg.substr(equalPos + 1);
//
//             options[option] = value;
//         }
//     }
// }
//
// void printOptions(const std::map<std::string, std::string>& options) {
//     for(const auto& pair : options) {
//         std::cout << pair.first << " : " << pair.second << std::endl;
//     }
// }
