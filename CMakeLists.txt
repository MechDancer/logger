cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(logger LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(fmt)

find_package(Threads REQUIRED)

add_library(logger src/logger_queue_t.hh src/logger_queue_t.cc)
target_link_libraries(logger fmt ${CMAKE_THREAD_LIBS_INIT})

add_executable(logger_example test/example.cpp)
target_link_libraries(logger_example logger)
