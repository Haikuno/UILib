cmake_minimum_required(VERSION 3.10)
project(UILib)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(lib)

add_executable(testuilib main.c)
target_link_libraries(testuilib uilib raylib libGimbal)
