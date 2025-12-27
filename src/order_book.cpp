/**
order_book.cpp
--------------
Implements OrderBook with a top of book query
and ability to add limit orders
 */

#include "order_book.hpp"

// OrderBook query function that returns whether there is a best ask
bool OrderBook::has_best_ask() const {
    return !asks.empty();
}

// OrderBook query function that returns whether there is a best bid
bool OrderBook::has_best_bid() const {
    return !bids.empty();
}

// Orderbook query function that returns the best bid price
int OrderBook::best_bid_price() const {
    return bids.begin()->first;
}

// Orderbook query function that returns the best ask price
int OrderBook::best_ask_price() const {
    return asks.begin()->first;
}

// Orderbook query function that returns the best bid quantity
int OrderBook::best_bid_quantity() const {
    return bids.begin()->second.total_qty;
}

// Orderbook query function that returns the best ask quantity
int OrderBook::best_ask_quantity() const {
    return asks.begin()->second.total_qty;
}

// Orderbook query function that returns the earliest best ask
const Order& OrderBook::best_ask_front() const {
    return asks.begin()->second.orders.front();
}

// Orderbook query function that returns the earliest best bid
const Order& OrderBook::best_bid_front() const {
    return bids.begin()->second.orders.front();
}

// Orderbook function to "execute" a specified qty of an the best ask
void OrderBook::consume_best_ask(int qty){
    if (!has_best_ask()) return;
    auto asks_it = asks.begin();
    auto order_it = asks_it->second.orders.begin();

    // while there is still qty to consume and there are orders at the best ask price
    while (qty > 0 && order_it != asks_it->second.orders.end()){
        
        // if qty >= qty of the first order
        if (qty >= order_it->qty_remaining){
            qty -= order_it->qty_remaining;
            asks_it->second.total_qty -= order_it->qty_remaining;
            order_it = asks_it->second.orders.erase(order_it);
            if (asks_it->second.orders.empty()){
                asks.erase(asks_it);
                break;
            }
        }
        else {
            order_it->qty_remaining -= qty;
            asks_it->second.total_qty -= qty;
            qty = 0;
        }
    }
}

// Orderbook function to "execute" a specified qty of an the best ask
void OrderBook::consume_best_bid(int qty){
    if (!has_best_bid()) return;

    auto bids_it = bids.begin();
    auto order_it = bids_it->second.orders.begin();

    // while there is still qty to consume and there are orders at the best ask price
    while (qty > 0 && order_it != bids_it->second.orders.end()){
        
        // if qty >= qty of the first order
        if (qty >= order_it->qty_remaining){
            qty -= order_it->qty_remaining;
            bids_it->second.total_qty -= order_it->qty_remaining;
            order_it = bids_it->second.orders.erase(order_it);
            if (bids_it->second.orders.empty()){
                bids.erase(bids_it);
                break;
            }
        }
        else {
            order_it->qty_remaining -= qty;
            bids_it->second.total_qty -= qty;
            qty = 0;
        }
    }
}


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

// OrderBook debugging function to return the top ask and bid
TopOfBook OrderBook::top_of_book() const{
    TopOfBook tob;

    if (has_best_bid()){
        PriceLevel bl{best_bid_price(), best_bid_quantity()};
        tob.best_bid = bl;
    }

    if (has_best_ask()){
        PriceLevel al{best_ask_price(), best_ask_quantity()};
        tob.best_ask = al;
    }

    return tob;
}

