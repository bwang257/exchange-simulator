/**
matching_engine.cpp
--------------
Implements the Matching Engine that
uses price-time priority matching
 */

#include "matching_engine.hpp"

using std::vector;


vector<Trade> MatchingEngine::process_new_order(int order_id, Side side, int price, int qty){
    vector<Trade> trades = order_match(order_id, side, price, qty);
    if (qty > 0) ob.add_limit(order_id, side, price, qty);
    return trades;
}

vector<Trade> order_match(int incoming_id, Side incoming_side, int incoming_price, int& remaining_qty){
    vector<Trade> trades;
    return trades;
}

