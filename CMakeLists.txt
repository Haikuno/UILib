cmake_minimum_required(VERSION 3.10)
project(UILib)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(libgimbal)

file(GLOB SRCS src/*.c src/*.h)

add_executable(uilib main.c ${SRCS})
target_link_libraries(uilib raylib libGimbal)
