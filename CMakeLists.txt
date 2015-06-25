cmake_minimum_required(VERSION 3.2)

project(divine C CXX)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(BINARY_DIR ${PROJECT_SOURCE_DIR}/src)
set(BINARY_FILE ${BINARY_DIR}/main.cpp)

find_package(Threads REQUIRED)

include_directories(${INCLUDE_DIR})

add_executable(divine ${BINARY_FILE})
target_link_libraries(divine ${CMAKE_THREAD_LIBS_INIT})
