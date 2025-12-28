/**
test_matching_engine.cpp
--------------
Implements simple unit tests for matching_engine.cpp
 */

#include "matching_engine.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;

int main(){
    MatchingEngine eng;
    vector<Trade> trades;
    TopOfBook tob;

    // Test adding resting orders
    trades = eng.process_new_order(1, Side::Buy, 104, 10);
    assert(trades.size() == 0);
    trades = eng.process_new_order(2, Side::Sell, 105, 6);
    assert(trades.size() == 0);
    tob = eng.top_of_book();
    assert(tob.best_ask.has_value());
    assert(tob.best_bid.has_value());

    // Buys:
    // 1: 104 @ 10
    // Sells:
    // 2: 105 @ 6

    // Single fill (full fill incoming/resting)
    trades = eng.process_new_order(3, Side::Buy, 110, 6);
    tob = eng.top_of_book();
    assert(trades.size() == 1);
    assert(trades[0].buy_id == 3);
    assert(trades[0].sell_id == 2);
    assert(trades[0].price == 105);
    assert(trades[0].qty == 6);
    assert(tob.best_bid.has_value());
    assert(!tob.best_ask.has_value());
    assert(tob.best_bid.value().price == 104);
    assert(tob.best_bid.value().qty == 10);

    // Buys:
    // 1: 104 @ 10
    // Sells:
    //

    // Partial fill of incoming, remainder rests
    trades = eng.process_new_order(4, Side::Sell, 103, 14);
    tob = eng.top_of_book();
    assert(trades.size() == 1);
    assert(trades[0].buy_id == 1);
    assert(trades[0].sell_id == 4);
    assert(trades[0].price == 104);
    assert(trades[0].qty == 10);
    assert(!tob.best_bid.has_value());
    assert(tob.best_ask.has_value());
    assert(tob.best_ask.value().price == 103);
    assert(tob.best_ask.value().qty == 4);

    // Buys:
    // 
    // Sells:
    // 4: 103 @ 4

    // Partial fill of resting (resting order survives)
    trades = eng.process_new_order(5, Side::Buy, 103, 2);
    tob = eng.top_of_book();
    assert(trades.size() == 1);
    assert(trades[0].buy_id == 5);
    assert(trades[0].sell_id == 4);
    assert(trades[0].price == 103);
    assert(trades[0].qty == 2);
    assert(!tob.best_bid.has_value());
    assert(tob.best_ask.has_value());
    assert(tob.best_ask.value().price == 103);
    assert(tob.best_ask.value().qty == 2);

    // Buys:
    // 
    // Sells:
    // 4: 103 @ 2

    // FIFO within a price level
    trades = eng.process_new_order(6, Side::Sell, 103, 3);
    assert(trades.empty());
    trades = eng.process_new_order(7, Side::Sell, 103, 2);
    assert(trades.empty());
    tob = eng.top_of_book();
    assert(tob.best_ask.value().price == 103);
    assert(tob.best_ask.value().qty == 7);

    trades = eng.process_new_order(8, Side::Buy, 104, 2);
    tob = eng.top_of_book();
    assert(trades.size() == 1);
    assert(trades[0].buy_id == 8);
    assert(trades[0].sell_id == 4);
    assert(trades[0].price == 103);
    assert(trades[0].qty == 2);

    trades = eng.process_new_order(9, Side::Buy, 104, 3);
    tob = eng.top_of_book();
    assert(trades.size() == 1);
    assert(trades[0].buy_id == 9);
    assert(trades[0].sell_id == 6);
    assert(trades[0].price == 103);
    assert(trades[0].qty == 3);

    trades = eng.process_new_order(10, Side::Buy, 104, 2);
    tob = eng.top_of_book();
    assert(trades.size() == 1);
    assert(trades[0].buy_id == 10);
    assert(trades[0].sell_id == 7);
    assert(trades[0].price == 103);
    assert(trades[0].qty == 2);
    assert(!tob.best_bid.has_value());
    assert(!tob.best_ask.has_value());

    // Buys:
    // 
    // Sells:
    //
    
    // Sweep across multiple price levels
    trades = eng.process_new_order(11, Side::Sell, 100, 4);
    assert(trades.empty());
    trades = eng.process_new_order(12, Side::Sell, 101, 5);
    assert(trades.empty());
    trades = eng.process_new_order(13, Side::Buy, 105, 7);
    tob = eng.top_of_book();
    assert(trades.size() == 2);
    assert(trades[0].buy_id == 13);
    assert(trades[0].sell_id == 11);
    assert(trades[0].price == 100);
    assert(trades[0].qty == 4);
    assert(trades[1].buy_id == 13);
    assert(trades[1].sell_id == 12);
    assert(trades[1].price == 101);
    assert(trades[1].qty == 3);
    assert(!tob.best_bid.has_value());
    assert(tob.best_ask.has_value());
    assert(tob.best_ask.value().price == 101);
    assert(tob.best_ask.value().qty == 2);

    cout << "test_matching_engine: PASS" << endl;

    return 0;

}
