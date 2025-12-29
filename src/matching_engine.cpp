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

void MatchingEngine::add_listener(IEventListener* l){
    listeners.push_back(l);
}

NewOrderResponse MatchingEngine::process_new_order(int order_id, Side side, int price, int qty){
    vector<Trade> trades;
    if (ob.has_order(order_id)){
        for (auto* l : listeners){
            l->on_reject(order_id, RejectReason::DUP);
        }
        return NewOrderResponse{false, RejectReason::DUP, trades};
    }
    if (price <= 0 || qty <= 0){
        for (auto* l : listeners){
            l->on_reject(order_id, RejectReason::BAD);
        }
        return NewOrderResponse{false, RejectReason::BAD, trades};
    }
    
    // Order Acknowledged
    for (auto* l : listeners){
        l->on_ack(order_id);
    }

    int remaining_qty = qty;
    trades = side == Side::Buy ? order_match_buy(order_id, price, remaining_qty) : order_match_sell(order_id, price, remaining_qty);
    for (const Trade& trade : trades){
        for (auto* l : listeners){
            l->on_trade(trade);
        }
    }
    
    if (remaining_qty > 0) ob.add_limit(order_id, side, price, remaining_qty);
    return NewOrderResponse{true, std::nullopt, trades};
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
    TopOfBook tob = ob.top_of_book();
    for (auto* l : listeners){
        l->on_tob(tob);
    }
    return tob;
}

BookSnapshot MatchingEngine::print_book() const{
    BookSnapshot bs = ob.print_book();
    for (auto * l : listeners){
        l->on_book(bs);
    }
    return bs;
}

CancelResult MatchingEngine::cancel_order(int order_id){
    CancelResult res = ob.cancel(order_id);
    for (auto* l : listeners){
        l->on_cancel(order_id, res);
    }
    return res;
}