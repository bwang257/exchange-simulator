/**
test_matching_cancel.cpp
--------------
Implements unit tests for order cancellation functionality
 */

#include "matching_engine.hpp"
#include "common.hpp"
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
    NewOrderResponse res;
    vector<Trade> trades;
    TopOfBook tob;

    // Set up initial state: one resting ask order
    res = eng.process_new_order(12, Side::Sell, 101, 2);
    assert(res.accepted == true);
    tob = eng.top_of_book();
    assert(!tob.best_bid.has_value());
    assert(tob.best_ask.has_value());
    assert(tob.best_ask.value().price == 101);
    assert(tob.best_ask.value().qty == 2);

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
    res = eng.process_new_order(14, Side::Buy, 100, 4);
    assert(res.accepted == true);
    res = eng.process_new_order(15, Side::Buy, 100, 7);
    assert(res.accepted == true);
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
    res = eng.process_new_order(16, Side::Buy, 100, 4);
    assert(res.accepted == true);
    res = eng.process_new_order(17, Side::Sell, 100, 4);
    assert(res.accepted == true);
    assert(res.trades.size() == 1);
    assert(eng.cancel_order(16) == CancelResult::Unknown);
    assert(eng.cancel_order(17) == CancelResult::Unknown);

    // Buys:
    //
    // Sells:
    // 

    // Cancel twice: first Cancelled, second Unknown
    res = eng.process_new_order(18, Side::Sell, 100, 4);
    assert(res.accepted == true);
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
    res = eng.process_new_order(19, Side::Sell, 100, 4);
    assert(res.accepted == true);
    assert(eng.cancel_order(19) == CancelResult::Cancelled);
    res = eng.process_new_order(20, Side::Buy, 100, 4);
    assert(res.accepted == true);
    assert(res.trades.size() == 0);
    assert(eng.cancel_order(20) == CancelResult::Cancelled);

    // Cancel partial quantity
    res = eng.process_new_order(21, Side::Sell, 100, 10);
    assert(res.accepted == true);
    res = eng.process_new_order(22, Side::Buy, 100, 6);
    assert(res.accepted == true);
    trades = res.trades;
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

    // Cancel middle order perserves FIFO of remaining orders
    res = eng.process_new_order(23, Side::Sell, 100, 1);
    assert(res.accepted == true);
    res = eng.process_new_order(24, Side::Sell, 100, 1);
    assert(res.accepted == true);
    res = eng.process_new_order(25, Side::Sell, 100, 1);
    assert(res.accepted == true);
    assert(eng.cancel_order(24) == CancelResult::Cancelled);
    res = eng.process_new_order(26, Side::Buy, 100, 1);
    assert(res.accepted == true);
    assert(res.trades[0].sell_id == 23);
    res = eng.process_new_order(27, Side::Buy, 100, 1);
    assert(res.accepted == true);
    assert(res.trades[0].sell_id == 25);
    assert(!tob.best_bid.has_value());
    assert(!tob.best_ask.has_value());

    cout << "test_matching_cancel: PASS" << endl;

    return 0;
}

