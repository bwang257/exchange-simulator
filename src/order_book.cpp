/**
order_book.cpp
--------------
Implements OrderBook with a top of book query
and ability to add limit orders
 */

#include "order_book.hpp"

// OrderBook function to add a new limit order to the orderbook
void OrderBook::add_limit(int order_id, Side side, int price, int qty){

    Order o{order_id, qty};
    if (side == Side::Buy){
        if (bids.count(price)){
            bids[price].orders.push_back(o);
            bids[price].total_qty+=qty;
            return;
        }
        Level l(o);
        bids.insert({price, l});
    }
    else {
        if (asks.count(price)){
            asks[price].orders.push_back(o);
            asks[price].total_qty +=qty;
            return;
        }
        Level l(o);
        asks.insert({price, l});
    }
}

// OrderBook function to return the top ask and bid
TopOfBook OrderBook::top_of_book() const{
    TopOfBook tob;
    
    if (!bids.empty()){
        PriceLevel bl{bids.begin()->first, bids.begin()->second.total_qty};
        tob.best_bid = bl;
    }

    if (!asks.empty()){
        PriceLevel al{asks.begin()->first, asks.begin()->second.total_qty};
        tob.best_ask  = al;
    }

    return tob;
}