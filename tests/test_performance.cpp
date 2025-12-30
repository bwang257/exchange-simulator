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

// Reads a file into a string
string read_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file " << filename << endl;
        return "";
    }
    
    ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void print_statistics(const std::vector<long long>& latencies, const string& operation_name, 
                      int operation_count, double total_wall_time_seconds) {
    if (latencies.empty()) {
        return;
    }

    // Sort latencies for percentile calculation
    std::vector<long long> sorted_latencies = latencies;
    std::sort(sorted_latencies.begin(), sorted_latencies.end());

    // Calculate mean latency
    long long sum = std::accumulate(sorted_latencies.begin(), sorted_latencies.end(), 0LL);
    double mean_ns = static_cast<double>(sum) / sorted_latencies.size();
    double mean_us = mean_ns / 1000.0;

    // Calculate P99 latency
    size_t p99_index = static_cast<size_t>((sorted_latencies.size() - 1) * 0.99);
    long long p99_ns = sorted_latencies[p99_index];
    double p99_us = p99_ns / 1000.0;

    // Calculate throughput: operations per second based on wall-clock time
    double throughput = operation_count / total_wall_time_seconds;

    cout << "=== " << operation_name << " Performance Statistics ===\n";
    cout << "Total Operations: " << operation_count << "\n";
    cout << "Mean Latency: " << mean_us << " us\n";
    cout << "P99 Latency: " << p99_us << " us\n";
    cout << "Throughput: " << throughput << " " << operation_name << "s/sec\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Please input: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    string input_file = argv[1];
    string input = read_file(input_file);
    if (input.empty()) {
        cerr << "Failed to read input file: " << input_file << endl;
        return 1;
    }

    MatchingEngine engine;

    // silent listener - output captured but not printed
    TestListener listener;
    engine.add_listener(&listener);

    // Latency storage vectors
    std::vector<long long> match_latencies;
    std::vector<long long> cancel_latencies;

    // Parse all commands first (exclude parsing from timing)
    std::vector<Command> commands;
    istringstream input_stream(input);
    string line;
    while (getline(input_stream, line)) {
        if (line == "X") break;
        commands.push_back(parse_command(line));
    }

    // Measure pure logic latency (excluding parsing/I/O)
    auto overall_start = std::chrono::high_resolution_clock::now();

    for (const auto& cmd : commands) {
        switch (cmd.type) {
            case CommandType::New: {
                // Start timer right before engine call
                ScopedTimer t(match_latencies);
                engine.process_new_order(cmd.order_id, cmd.side, cmd.price, cmd.qty);
                // Timer stops automatically when scope ends
                break;
            }
            case CommandType::Cancel: {
                ScopedTimer t(cancel_latencies);
                engine.cancel_order(cmd.order_id);
                break;
            }
            case CommandType::PrintTopOfBook:
                engine.top_of_book();
                break;
            case CommandType::PrintFullBook:
                engine.print_book();
                break;
            case CommandType::Exit:
                break;
            default:
                break;
        }
    }

    auto overall_end = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(overall_end - overall_start);
    double total_seconds = total_duration.count() / 1e9;

    // Print statistics
    if (!match_latencies.empty()) {
        print_statistics(match_latencies, "Order", match_latencies.size(), total_seconds);
        cout << "\n";
    }
    if (!cancel_latencies.empty()) {
        print_statistics(cancel_latencies, "Cancel", cancel_latencies.size(), total_seconds);
        cout << "\n";
    }

    return 0;
}
