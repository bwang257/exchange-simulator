/**
test_order_book.cpp
--------------
Implements simple unit tests for order_book.cpp
 */

#include "order_book.hpp"
#include <cassert>
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

int main(){

    OrderBook ob;
    TopOfBook tob = ob.top_of_book();
    assert(!tob.best_ask.has_value());
    assert(!tob.best_bid.has_value());

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

    assert(tob.best_ask->price == 103);
    assert(tob.best_ask->qty == 11);
    assert(tob.best_bid->price == 103);
    assert(tob.best_bid->qty == 7);

    // tob updates so that best bid price != ask price
    ob.add_limit(8, Side::Buy, 104, 6);
    tob = ob.top_of_book();
    assert(tob.best_bid->price == 104);
    assert(tob.best_bid->qty == 6);

    cout << "test_order_book: PASS" << endl;
    return 0;
}






