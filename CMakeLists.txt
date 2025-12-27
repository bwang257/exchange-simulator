cmake_minimum_required(VERSION 3.16)
project(exchange_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Recommended Warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Parser library
add_library(parser src/parser.cpp)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# OrderBook library
add_library(orderbook src/order_book.cpp)
target_include_directories(orderbook PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(orderbook PUBLIC parser)

# Parser tests
add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser PRIVATE parser)

# OrderBook tests
add_executable(test_order_book tests/test_order_book.cpp)
target_link_libraries(test_order_book PRIVATE orderbook)
