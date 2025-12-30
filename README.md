# Exchange Simulator (C++)

A simplified single-symbol limit order book exchange simulator implemented in C++17. Features price-time priority matching, order cancellation, and event-driven architecture.

## Features

**Core Functionality:**
- Limit order book with FIFO queues per price level
- Price-time priority order matching
- Trade execution with automatic order matching
- Partial fills support
- Order cancellation with FIFO preservation
- Top-of-book and full book queries

**Architecture:**
- Event-driven design (Observer pattern)
- Clean separation of concerns
- Batch file processing
- Interactive and file input modes

**Testing:**
- Comprehensive unit test coverage
- Golden test framework for regression testing
- Automated test file generation

## Quick Start

```bash
# Build the project
mkdir build && cmake -S . -B build && cmake --build build

# Run interactively
./build/exchange_simulator

# Process a file
./build/exchange_simulator input.txt

# Run tests
./build/test_parser
./build/test_order_book
./build/test_matching_basic
./build/test_matching_cancel
```

## Build

**Requirements:**
- CMake ≥ 3.16
- C++17 compatible compiler (GCC, Clang, MSVC)

**Build Commands:**
```bash
mkdir build
cmake -S . -B build
cmake --build build
```

## Usage

### Interactive Mode
Read commands from stdin:
```bash
./build/exchange_simulator
# Then type commands:
N 1 B 100 10
N 2 S 105 5
P
X
```

### File Mode
Process commands from a file:
```bash
./build/exchange_simulator input.txt
```

## Command Format

| Command | Format | Description |
|---------|--------|-------------|
| **N** | `N <order_id> <side> <price> <qty>` | New limit order |
| **C** | `C <order_id>` | Cancel order |
| **P** | `P` | Print top of book (best bid/ask) |
| **B** | `B` | Print full book (all price levels) |
| **X** | `X` | Exit |

**Examples:**
- `N 1 B 100 10` - Buy order: ID=1, price=100, qty=10
- `N 2 S 105 5` - Sell order: ID=2, price=105, qty=5
- `C 1` - Cancel order ID 1
- `P` - Show best bid and ask

**Validation:**
- Order IDs must be positive integers
- Prices must be positive integers
- Quantities must be positive integers
- Side must be exactly "B" (buy) or "S" (sell)

## Output Format

The simulator outputs events as they occur:

| Event | Format | Description |
|-------|--------|-------------|
| **ACK** | `ACK <order_id>` | Order accepted |
| **TRD** | `TRD <buy_id> <sell_id> <price> <qty>` | Trade executed |
| **REJ** | `REJ <order_id> <reason>` | Order rejected (BAD/DUP/UNK) |
| **CXL** | `CXL <order_id>` | Order cancelled |
| **TOB** | `TOB BID <price> <qty>` | Top of book bid |
| **TOB** | `TOB ASK <price> <qty>` | Top of book ask |
| **BOOK** | `BOOK BID <price> <qty>` | Full book bid level |
| **BOOK** | `BOOK ASK <price> <qty>` | Full book ask level |

**Example Output:**
```
ACK 1
ACK 2
TRD 1 2 100 5
TOB BID 100 5
TOB ASK 105 5
```

## Testing

### Unit Tests
Run all unit tests:
```bash
./build/test_parser
./build/test_order_book
./build/test_matching_basic
./build/test_matching_cancel
```

### Golden Tests
Golden tests compare actual output against expected reference files.

**Generate Test Files:**
```bash
# Generate random test input
python3 scripts/generate_test.py -n 1000 -o test_input.txt --seed 42

# Generate expected output (golden reference)
./build/exchange_simulator test_input.txt > test_expected.txt
```

**Run Golden Tests:**
```bash
./build/test_golden <input_file> <expected_file>
```

**Pre-generated Test Files:**
```bash
./build/test_golden tests/data/tight_spread.txt tests/data/tight_spread_expected.txt
./build/test_golden tests/data/benchmark_100k.txt tests/data/benchmark_100k_expected.txt
```

## Project Status

**Completed Milestones:**
- **Milestone 1**: Command parsing and validation
- **Milestone 2**: Limit order book with FIFO queues per price level
- **Milestone 3**: Matching engine with price-time priority matching
- **Milestone 4**: Order cancellation + Event system (Observer pattern)
- **Milestone 5**: File batch processing and golden tests

**Project Structure:**
```
exchange_simulator/
├── src/              # Source code
│   ├── parser.*      # Command parsing
│   ├── order_book.*  # Order book implementation
│   ├── matching_engine.*  # Matching logic
│   └── main.cpp      # Application entry point
├── tests/            # Test files
│   ├── test_*.cpp    # Unit tests
│   └── data/         # Test data files
├── scripts/          # Utility scripts
│   └── generate_test.py  # Test file generator
└── build/            # Build output (generated)
```

**Technology:**
- C++17 standard
- CMake build system
- Observer pattern for event handling
- STL containers (map, list, unordered_map)