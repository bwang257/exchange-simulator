/**
matching_engine.cpp
--------------
Implements the Matching Engine that
uses price-time priority matching
 */

#include "matching_engine.hpp"
#include <algorithm>

using std::vector;
using std::min;


vector<Trade> MatchingEngine::process_new_order(int order_id, Side side, int price, int qty){
    vector<Trade> trades = side == Side::Buy ? order_match_buy(order_id, price, qty) : order_match_sell(order_id, price, qty);
    if (qty > 0) ob.add_limit(order_id, side, price, qty);
    return trades;
}

vector<Trade> MatchingEngine::order_match_buy(int incoming_id, int incoming_price, int& remaining_qty){
    vector<Trade> trades;
    while (remaining_qty > 0 && ob.has_best_ask()){
        int price = ob.best_ask_price();
        if (price > incoming_price){
            break;
        }
        int fill_qty = min(ob.best_ask_quantity(), remaining_qty);
        vector<Fill> completed_fills = ob.consume_best_ask(fill_qty);
        for (Fill f : completed_fills){
            trades.push_back(Trade{incoming_id, f.resting_order_id, price, f.qty_filled});
            remaining_qty -= f.qty_filled;
        }
    }
    return trades; 
}

vector<Trade> MatchingEngine::order_match_sell(int incoming_id, int incoming_price, int& remaining_qty){
    vector<Trade> trades;
    while (remaining_qty > 0 && ob.has_best_bid()){
        int price = ob.best_bid_price();
        if (price < incoming_price){
            break;
        }
        int fill_qty = min(ob.best_bid_quantity(), remaining_qty);
        vector<Fill> completed_fills = ob.consume_best_bid(fill_qty);
        for (Fill f : completed_fills){
            trades.push_back(Trade{f.resting_order_id, incoming_id, price, f.qty_filled});
            remaining_qty -= f.qty_filled;
        }
    }
    return trades;
}


TopOfBook MatchingEngine::top_of_book() const{
    return ob.top_of_book();
}
