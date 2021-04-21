cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(pm1_sdk LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Release")

add_subdirectory(fmt)
include_directories(src)

find_package(Threads REQUIRED)

add_library(logger src/logger_queue_t.hh src/logger_queue_t.cc)
target_link_libraries(logger fmt ${CMAKE_THREAD_LIBS_INIT})

add_executable(test test/main.cpp)
target_link_libraries(test logger)
