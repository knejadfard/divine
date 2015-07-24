cmake_minimum_required(VERSION 3.2)

project(divine CXX)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(BINARY_DIR ${PROJECT_SOURCE_DIR}/src)
set(BINARY_FILE ${BINARY_DIR}/main.cpp)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif()

find_package(Threads REQUIRED)

include_directories(${INCLUDE_DIR})

add_executable(divine ${BINARY_FILE})
