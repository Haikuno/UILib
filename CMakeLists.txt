cmake_minimum_required(VERSION 3.10)
project(UILib)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(GUM_ENABLE_EXAMPLES        "Build examples"        OFF)
option(GUM_ENABLE_TESTS           "Build unit tests"      OFF)
option(GUM_ENABLE_BACKEND_SDL3    "Enable SDL3 backend"   OFF)
option(GUM_ENABLE_BACKEND_RAYLIB  "Enable raylib backend"  ON)

if(GUM_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GUM_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

add_subdirectory(lib)

add_executable(testlibGumball main.c)
target_link_libraries(testlibGumball libGumball)
