cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(pm1_sdk LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Release")

add_subdirectory(fmt)
include_directories(src)

# find_package(Threads REQUIRED)

add_executable(main src/compiling_string.hpp src/logger.hpp src/main.cpp)
target_link_libraries(main fmt)
# target_link_libraries(console kernel -latomic quill::quill ${CMAKE_THREAD_LIBS_INIT})

EXECUTE_PROCESS(COMMAND uname -m OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE architecture)
if(architecture STREQUAL "aarch64")
else()
endif()
