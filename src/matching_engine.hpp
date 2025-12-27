/**
matching_engine.hpp
--------------
Defines the Matching Engine interface and Trade struct
 */

#pragma once

#include "order_book.hpp"
#include <vector>

struct Trade {
    int buy_id;
    int sell_id;
    int price;
    int qty;
};


class MatchingEngine{
private:
    OrderBook ob;

public:
    std::vector<Trade> process_new_order(int order_id, Side side, int price, int qty);
};