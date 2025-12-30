/**
test_performance.cpp
--------------
Measures latency and throughput using RAII ScopedTimer
 */

#include "matching_engine.hpp"
#include "parser.hpp"
#include "test_listener.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>

using std::string;
using std::ifstream;
using std::istringstream;
using std::getline;
using std::cerr;
using std::cout;
using std::endl;
using std::ostringstream;

// RAII ScopedTimer for clean benchmarking
struct ScopedTimer {
    std::vector<long long>& latencies;
    std::chrono::high_resolution_clock::time_point start;

    ScopedTimer(std::vector<long long>& storage) 
        : latencies(storage), start(std::chrono::high_resolution_clock::now()) {}

    ~ScopedTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        latencies.push_back(duration);
    }
};