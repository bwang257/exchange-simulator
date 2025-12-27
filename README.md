# Exchange Simulator (C++)

A simplified single-symbol exchange simulator implemented in C++.

Current Status
- Milestone 1 complete: command parsing + validation
- Supported commands: P, B, X, C, N

Build
- Requires CMake ≥ 3.16 and a C++17 compiler
- mkdir build && cmake -S . -B build && cmake --build build

Tests
- ./build/test_parser
- ./build/test_order_book
