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
    std::vector<Trade> order_match_buy(int incoming_id, int incoming_price, int& remaining_qty);
    std::vector<Trade> order_match_sell(int incoming_id, int incoming_price, int& remaining_qty);

public:
    std::vector<Trade> process_new_order(int order_id, Side side, int price, int qty);
    TopOfBook top_of_book() const;
    CancelResult cancel_order(int order_id);
};