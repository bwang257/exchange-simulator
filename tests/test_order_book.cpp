/**
test_order_book.cpp
--------------
Implements simple unit tests for order_book.cpp
 */

#include "order_book.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;

int main(){

    // empty order book
    OrderBook ob;
    TopOfBook tob = ob.top_of_book();
    assert(!tob.best_ask.has_value());
    assert(!tob.best_bid.has_value());
    assert(ob.has_best_ask() == false);
    assert(ob.has_best_bid() == false);
    
    // consume best ask and bid when empty
    vector<Order> fills = ob.consume_best_ask(5);
    assert(fills.size() == 0);
    fills = ob.consume_best_bid(5);
    assert(fills.size() == 0);

    // add bids
    ob.add_limit(1, Side::Buy, 100, 5);
    ob.add_limit(2, Side::Buy, 100, 10);
    ob.add_limit(3, Side::Buy, 103, 7);

    // add asks
    ob.add_limit(4, Side::Sell, 109, 5);
    ob.add_limit(5, Side::Sell, 120, 3);
    ob.add_limit(6, Side::Sell, 103, 8);
    ob.add_limit(7, Side::Sell, 103, 3);

    // check if top of book is accurate
    tob = ob.top_of_book();
    assert(tob.best_ask.has_value());
    assert(tob.best_bid.has_value());

    // check if queries are accurate
    // best ask: 103, 11
    // best bid: 103, 7
    assert(tob.best_ask->price == 103);
    assert(tob.best_ask->qty == 11);
    assert(tob.best_bid->price == 103);
    assert(tob.best_bid->qty == 7);
    assert(ob.has_best_ask() == true);
    assert(ob.has_best_bid() == true);
    assert(ob.best_ask_price() == 103);
    assert(ob.best_bid_price() == 103);
    assert(ob.best_ask_quantity() == 11);
    assert(ob.best_bid_quantity() == 7);
    assert(ob.best_ask_front().order_id == 6);
    assert(ob.best_bid_front().order_id == 3);
    assert(ob.best_ask_front().qty_remaining == 8);
    assert(ob.best_bid_front().qty_remaining == 7);

    // tob updates so that best bid price != ask price
    ob.add_limit(8, Side::Buy, 104, 6);
    tob = ob.top_of_book();
    assert(tob.best_bid->price == 104);
    assert(tob.best_bid->qty == 6);

    // check consume best ask
    // best ask: 103, 11
    // best bid: 104, 6
    fills = ob.consume_best_ask(5);
    assert(fills.size() == 1);
    assert(fills[0].order_id == 6);
    assert(fills[0].qty_remaining == 5);
    assert(ob.best_ask_price() == 103);
    assert(ob.best_ask_quantity() == 6);
    ob.add_limit(9, Side::Sell, 103, 6);

    // order 6: qty remaining 3
    // order 7: qty remaining 3
    // order 9: qty remaining 6
    assert(ob.best_ask_quantity() == 12);
    fills = ob.consume_best_ask(7);
    assert(fills.size() == 3);
    assert(fills[0].order_id == 6);
    assert(fills[0].qty_remaining == 3);
    assert(fills[1].order_id == 7);
    assert(fills[1].qty_remaining == 3);
    assert(fills[2].order_id == 9);
    assert(fills[2].qty_remaining == 1);

    assert(ob.best_ask_quantity() == 5);
    fills = ob.consume_best_ask(5);
    assert(fills.size() == 1);
    assert(fills[0].order_id == 9);
    assert(fills[0].qty_remaining == 5); 
    assert(ob.has_best_ask());
    assert(ob.best_ask_price() == 109);
    assert(ob.best_ask_quantity() == 5);

    // check consume best bid
    // best ask: 109, 5
    // best bid: 104, 6
    ob.consume_best_bid(4);
    assert(ob.best_bid_price() == 104);
    assert(ob.best_bid_quantity() == 2);
    ob.consume_best_bid(2);
    assert(ob.best_bid_price() == 103);
    assert(ob.best_bid_quantity() == 7);
    ob.consume_best_bid(8);
    assert(ob.best_bid_price() == 100);
    assert(ob.best_bid_quantity() == 15);

    ob.add_limit(10, Side::Buy, 103, 10);
    fills = ob.consume_best_bid(99999);
    assert(fills.size() == 1);
    assert(fills[0].order_id == 10);
    assert(fills[0].qty_remaining == 10); 
    assert(ob.best_bid_price() == 100);
    assert(ob.best_bid_quantity() == 15);

    cout << "test_order_book: PASS" << endl;
    return 0;
}






