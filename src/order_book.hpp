/**
order_book.hpp
--------------
Defines the OrderBook interface and PriceLevel and TopOfBook Structs
Implements FIFO order queues per price level
 */

#pragma once

#include <map>
#include <deque>
#include <functional>
#include <optional>
#include "parser.hpp"
 

struct PriceLevel {
    int price;
    int qty;
};

struct Order {
    int order_id;
    int qty_remaining;
};

struct Level {
    int total_qty = 0;
    std::deque<Order> orders;
};

struct TopOfBook {
    std::optional<PriceLevel> best_ask;
    std::optional<PriceLevel> best_bid;

};

class OrderBook {
private:
    std::map<int, Level> asks;
    std::map<int, Level, std::greater<int>> bids;

public: 
    void add_limit(int order_id, Side side, int price, int qty);
    TopOfBook top_of_book() const;
};
