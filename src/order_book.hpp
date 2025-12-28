/**
order_book.hpp
--------------
Defines the OrderBook interface and PriceLevel and TopOfBook Structs
Implements FIFO order queues per price level
 */

#pragma once
#include <map>
#include <unordered_map>
#include <list>
#include <functional>
#include <optional>
#include "common.hpp"
 
struct PriceLevel {
    int price;
    int qty;
};

struct Fill { 
    int resting_order_id;
    int qty_filled;
};

struct Order {
    int order_id;
    int qty_remaining;
};

struct Level {
    int total_qty = 0;
    std::list<Order> orders;

    Level() = default;

    Level(Order o){
        orders.push_back(o);
        total_qty+= o.qty_remaining;
    }
};

struct Location {
    Side side;
    int price;
    std::list<Order>::iterator order_it;
};

struct TopOfBook {
    std::optional<PriceLevel> best_ask;
    std::optional<PriceLevel> best_bid;
};

enum class CancelResult {
    Cancelled,
    Unknown
};

class OrderBook {
private:
    std::map<int, Level> asks;
    std::map<int, Level, std::greater<int>> bids;
    std::unordered_map<int, Location> live_orders;

public: 
    void add_limit(int order_id, Side side, int price, int qty);
    TopOfBook top_of_book() const;

    bool has_best_ask() const;
    bool has_best_bid() const;

    int best_ask_price() const;
    int best_bid_price() const;
    int best_ask_quantity() const;
    int best_bid_quantity() const;

    const Order& best_ask_front() const;
    const Order& best_bid_front() const;

    std::vector<Fill> consume_best_ask(int qty);
    std::vector<Fill> consume_best_bid(int qty);

    CancelResult cancel(int order_id);
};
