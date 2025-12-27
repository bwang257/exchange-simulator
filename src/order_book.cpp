/**
order_book.cpp
--------------
Implements OrderBook with a top of book query
and ability to add limit orders
 */

#include "order_book.hpp"

// OrderBook function to add a new limit order to the orderbook
void OrderBook::add_limit(int order_id, Side side, int price, int qty){

    if (side == Side::Buy){
        if (bids.count(price)){
            bids[price]+=qty;
            return;
        }
        bids.insert({price, qty});
    }
    else {
        if (asks.count(price)){
            asks[price]+=qty;
            return;
        }
        asks.insert({price, qty});
    }
}

// OrderBook function to return the top ask and bid
TopOfBook OrderBook::top_of_book() const{
    TopOfBook tob;
    
    if (!bids.empty()){
        PriceLevel bl{bids.begin()->first, bids.begin()->second};
        tob.best_bid = bl;
    }

    if (!asks.empty()){
        PriceLevel al{asks.begin()->first, asks.begin()->second};
        tob.best_ask  = al;
    }

    return tob;
}