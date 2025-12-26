cmake_minimum_required(VERSION 3.16)
project(exchange_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Recommended Warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Build parser as a library so both app/tests can link it
add_library(parser src/parser.cpp)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Test executable
add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser PRIVATE parser)
