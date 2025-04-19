cmake_minimum_required(VERSION 3.10)

project(EOS VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
    src/main.cpp
    src/include/utils/ip_util.cpp
    src/include/utils/options.cpp
    src/include/server/start_server.cpp
    src/include/server/ctc.cpp
    src/include/server/srmc.cpp
    src/include/server/handler.cpp
)

find_package(CURL REQUIRED)

add_executable(open ${SOURCES})


target_link_libraries(open CURL::libcurl)

