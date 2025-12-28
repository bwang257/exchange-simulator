# Exchange Simulator (C++)

A simplified single-symbol exchange simulator implemented in C++.

## Current Status
- **Milestone 1 complete**: Command parsing and validation
- **Milestone 2 complete**: Limit order book with FIFO queues per price level
- **Milestone 3 complete**: Matching engine with price-time priority matching

## Features
- Command parsing (P, B, X, C, N)
- Limit order book with best bid/ask queries
- Price-time priority order matching
- Trade execution with automatic order matching
- Partial fills support

## Build
Requires CMake ≥ 3.16 and a C++17 compiler:
mkdir build && cmake -S . -B build && cmake --build build

## Tests
Run all tests:
./build/test_parser
./build/test_order_book
./build/test_matching_engine