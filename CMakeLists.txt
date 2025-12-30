cmake_minimum_required(VERSION 3.16)
project(exchange_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Recommended Warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Main executable
add_executable(exchange_simulator src/main.cpp)
target_link_libraries(exchange_simulator PRIVATE matching_engine parser)

# Parser library
add_library(parser src/parser.cpp)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Parser tests
add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser PRIVATE parser)

# OrderBook library
add_library(orderbook src/order_book.cpp)
target_include_directories(orderbook PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# OrderBook tests
add_executable(test_order_book tests/test_order_book.cpp)
target_link_libraries(test_order_book PRIVATE orderbook)

# Matching Engine library
add_library(matching_engine src/matching_engine.cpp)
target_include_directories(matching_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(matching_engine PUBLIC orderbook)

# Matching Engine tests
add_executable(test_matching_basic tests/test_matching_basic.cpp)
target_link_libraries(test_matching_basic PRIVATE matching_engine)

add_executable(test_matching_cancel tests/test_matching_cancel.cpp)
target_link_libraries(test_matching_cancel PRIVATE matching_engine)

# Golden tests
add_executable(test_golden tests/test_golden.cpp)
target_link_libraries(test_golden PRIVATE matching_engine parser)
target_include_directories(test_golden PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Performance tests
add_executable(test_performance tests/test_performance.cpp)
target_link_libraries(test_performance PRIVATE matching_engine parser)
target_include_directories(test_performance PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/tests)
