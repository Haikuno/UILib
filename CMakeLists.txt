cmake_minimum_required(VERSION 3.10)
project(UILib)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)

add_subdirectory(libgimbal)

file(GLOB SRCS src/*.c src/*.h)

add_executable(uilib main.c ${SRCS})
target_link_libraries(uilib raylib libGimbal)
