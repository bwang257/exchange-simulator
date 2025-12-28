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

# Parser tests
add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser PRIVATE parser)

# OrderBook library
add_library(orderbook src/order_book.cpp)
target_include_directories(orderbook PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(orderbook PUBLIC parser)

# OrderBook tests
add_executable(test_order_book tests/test_order_book.cpp)
target_link_libraries(test_order_book PRIVATE orderbook)

# Matching Engine library
add_library(matching_engine src/matching_engine.cpp)
target_include_directories(matching_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(matching_engine PUBLIC orderbook)

# Matching Engine tests
add_executable(test_matching_engine tests/test_matching_engine.cpp)
target_link_libraries(test_matching_engine PRIVATE matching_engine)
