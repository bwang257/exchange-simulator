/**
performance_profiler.hpp
--------------
Performance profiling wrapper for MatchingEngine
Tracks latency and throughput metrics
 */

#pragma once

#include "matching_engine.hpp"
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

class PerformanceProfiler {
private:
    MatchingEngine engine;
    std::vector<long long> latencies_ns;
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    int order_cnt;

public:
    PerformanceProfiler() : order_cnt(0) {}

    // Wrapper for process_new_order that measures latency
    NewOrderResponse process_new_order(int order_id, Side side, int price, int qty) {
        auto begin = std::chrono::high_resolution_clock::now();
        
        NewOrderResponse response = engine.process_new_order(order_id, side, price, qty);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        
        latencies_ns.push_back(duration.count());
        order_cnt++;
        
        // if first order
        if (order_cnt == 1) {
            start_time = begin;
        }

        // update time of last order
        end_time = end;
        
        return response;
    }

    // Wrapper for other MatchingEngine methods
    TopOfBook top_of_book() const {
        return engine.top_of_book();
    }

    BookSnapshot print_book() const {
        return engine.print_book();
    }

    CancelResult cancel_order(int order_id) {
        return engine.cancel_order(order_id);
    }

    void add_listener(IEventListener* listener) {
        engine.add_listener(listener);
    }

    // Print performance statistics
    void print_statistics() const {
        if (latencies_ns.empty()) {
            std::cout << "No orders processed.\n";
            return;
        }

        // Sort latencies for percentile calculation
        std::vector<long long> sorted_latencies = latencies_ns;
        std::sort(sorted_latencies.begin(), sorted_latencies.end());

        // Calculate mean latency
        long long sum = std::accumulate(sorted_latencies.begin(), sorted_latencies.end(), 0LL);
        double mean_ns = static_cast<double>(sum) / sorted_latencies.size();
        double mean_us = mean_ns / 1000.0;  // Convert to ms

        // Calculate P99 latency
        // For n items, P99 is at index floor((n-1) * 0.99)
        size_t p99_index = static_cast<size_t>((sorted_latencies.size() - 1) * 0.99);
        long long p99_ns = sorted_latencies[p99_index];
        double p99_us = p99_ns / 1000.0;  // Convert to ms

        // Calculate total time and throughput
        auto total_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            end_time - start_time);
        double total_seconds = total_duration.count() / 1e9;
        double throughput = order_cnt / total_seconds;  // Orders per second

        // Print statistics
        std::cout << "=== Performance Statistics ===\n";
        std::cout << "Total Orders: " << order_cnt << "\n";
        std::cout << "Total Time: " << total_seconds * 1000.0 << " ms\n";
        std::cout << "Mean Latency: " << mean_us << " us\n";
        std::cout << "P99 Latency: " << p99_us << " us\n";
        std::cout << "Throughput: " << throughput << " orders/sec\n";
    }

    // Raw statistics
    int get_order_count() const { return order_cnt; }
    const std::vector<long long>& get_latencies() const { return latencies_ns; }
    
    void reset() {
        latencies_ns.clear();
        order_cnt = 0;
    }
};

