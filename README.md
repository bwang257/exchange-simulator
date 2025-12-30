# Exchange Simulator (C++)

A simplified single-symbol exchange simulator implemented in C++.

## Current Status
- **Milestone 1 complete**: Command parsing and validation
- **Milestone 2 complete**: Limit order book with FIFO queues per price level
- **Milestone 3 complete**: Matching engine with price-time priority matching
- **Milestone 4 complete**: Order cancellation + Event system (Observer pattern)
- **Milestone 5 complete**: File batch processing and golden tests

## Features
- Command parsing (P, B, X, C, N)
- Limit order book with best bid/ask queries
- Price-time priority order matching
- Trade execution with automatic order matching
- Partial fills support
- Order cancellation with FIFO preservation
- Event-driven architecture (Observer pattern)
- Fully integrated application with command processing
- Batch file processing (command-line file input)
- Golden test framework for regression testing

## Build
Requires CMake ≥ 3.16 and a C++17 compiler:
mkdir build && cmake -S . -B build && cmake --build build

## Tests
Run all tests:
./build/test_parser
./build/test_order_book
./build/test_matching_basic
./build/test_matching_cancel

# Golden Test
Generate input test file:
python3 scripts/generate_test.py -n <num_orders> -o <input_file.txt> --seed 42

Generate expected output file:
./build/exchange_simulator <input_file.txt> > <expected_file.txt>

Run golden tests (requires expected output files):
./build/test_golden <input_file.txt> <expected_file.txt>

Example:
./build/test_golden tight_spread.txt tight_spread_expected.txt
./build/test_golden benchmark_100k.txt benchmark_100k_expected.txt