cmake_minimum_required(VERSION 3.10)
project(UILib)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(libgimbal)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS	  src/*.c)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS   api/ui/*.h)

add_library(uilib STATIC ${SRCS} ${HEADERS})
target_link_libraries(uilib raylib libGimbal)
target_include_directories(uilib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/api
)
