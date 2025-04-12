cmake_minimum_required(VERSION 3.10)

project(EOS VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
    src/include/utils/ip_util.cpp
    src/include/utils/options.cpp
    src/include/server/start_server.cpp
    src/include/server/oto.cpp
)

add_executable(easily-open-server ${SOURCES})

