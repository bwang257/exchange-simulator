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

    // Buys:
    // 
    // Sells:
    // 12: 101 @ 2

    // Cancel unknown order
    assert(eng.cancel_order(999) == CancelResult::Unknown);
    tob = eng.top_of_book();
    assert(!tob.best_bid.has_value());
    assert(tob.best_ask.has_value());
    assert(tob.best_ask.value().price == 101);
    assert(tob.best_ask.value().qty == 2);

    // remove single resting order
    assert(eng.cancel_order(12) == CancelResult::Cancelled);
    tob = eng.top_of_book();
    assert(!tob.best_bid.has_value());
    assert(!tob.best_ask.has_value());

    // Buys:
    // 
    // Sells:
    // 

    // cancel reduces aggregated qty at the level
    trades = eng.process_new_order(14, Side::Buy, 100, 4);
    trades = eng.process_new_order(15, Side::Buy, 100, 7);
    tob = eng.top_of_book();
    assert(tob.best_bid.has_value());
    assert(!tob.best_ask.has_value());
    assert(tob.best_bid.value().price == 100);
    assert(tob.best_bid.value().qty == 11);
    assert(eng.cancel_order(15) == CancelResult::Cancelled);
    tob = eng.top_of_book();
    assert(tob.best_bid.has_value());
    assert(!tob.best_ask.has_value());
    assert(tob.best_bid.value().price == 100);
    assert(tob.best_bid.value().qty == 4);

    // Buys:
    // 14: 100 @ 4
    // Sells:
    // 

    // Cancel removes the entire price level if it becomes empty
    assert(eng.cancel_order(14) == CancelResult::Cancelled);
    tob = eng.top_of_book();
    assert(!tob.best_bid.has_value());
    assert(!tob.best_ask.has_value());

    // Cancel after full fill returns Unknown
    trades = eng.process_new_order(16, Side::Buy, 100, 4);
    trades = eng.process_new_order(17, Side::Sell, 100, 4);
    assert(trades.size() == 1);
    assert(eng.cancel_order(16) == CancelResult::Unknown);
    assert(eng.cancel_order(17) == CancelResult::Unknown);

    // Buys:
    //
    // Sells:
    // 

    // Cancel twice: first Cancelled, second Unknown
    trades = eng.process_new_order(18, Side::Sell, 100, 4);
    tob = eng.top_of_book();
    assert(!tob.best_bid.has_value());
    assert(tob.best_ask.has_value());
    assert(tob.best_ask.value().price == 100);
    assert(tob.best_ask.value().qty == 4);
    assert(eng.cancel_order(18) == CancelResult::Cancelled);
    tob = eng.top_of_book();
    assert(!tob.best_bid.has_value());
    assert(!tob.best_ask.has_value());
    assert(eng.cancel_order(18) == CancelResult::Unknown);
    tob = eng.top_of_book();
    assert(!tob.best_bid.has_value());
    assert(!tob.best_ask.has_value());

    // Cancel affects future trades
    trades = eng.process_new_order(19, Side::Sell, 100, 4);
    assert(eng.cancel_order(19) == CancelResult::Cancelled);
    trades = eng.process_new_order(20, Side::Buy, 100, 4);
    assert(trades.size() == 0);
     assert(eng.cancel_order(20) == CancelResult::Cancelled);

    // Cancel partial quantity
    trades = eng.process_new_order(21, Side::Sell, 100, 10);
    trades = eng.process_new_order(22, Side::Buy, 100, 6);
    assert(trades.size() == 1);
    assert(trades[0].qty == 6);
    assert(trades[0].buy_id == 22);
    assert(trades[0].sell_id == 21);

    tob = eng.top_of_book();
    assert(!tob.best_bid.has_value());
    assert(tob.best_ask.has_value());
    assert(tob.best_ask.value().price == 100);
    assert(tob.best_ask.value().qty == 4);
    assert(eng.cancel_order(21) == CancelResult::Cancelled);
    tob = eng.top_of_book();
    assert(!tob.best_bid.has_value());
    assert(!tob.best_ask.has_value());

    cout << "test_matching_engine: PASS" << endl;

    return 0;
}
