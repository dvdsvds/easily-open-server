cmake_minimum_required(VERSION 3.10)

project(EOS VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
)

add_executable(easily-open-server ${SOURCES})

