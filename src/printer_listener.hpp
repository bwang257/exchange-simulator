/**
printer_listener.hpp
--------------
Defines PrinterListener for outputting events to stdout
 */

#pragma once

#include "events.hpp"
#include "common.hpp"
#include <iostream>

using std::cout;
using std::endl;

struct PrinterListener : IEventListener {
    
    void on_ack(int order_id) override {
        cout << "ACK " << order_id << endl;
    }

    void on_reject(int order_id, RejectReason rr) override{
        if (rr == RejectReason::BAD){
            cout << "REJ " << order_id << " BAD" << endl;
        }
        else if (rr == RejectReason::DUP){
            cout << "REJ " << order_id << " DUP" << endl;
        }
    }

    void on_trade(const Trade& trd) override{
        cout << "TRD " << trd.buy_id << " " << trd.sell_id << " " << trd.price << " " << trd.qty << endl;
    }

    void on_cancel(int order_id, CancelResult cr) override {
        if (cr == CancelResult::Cancelled){
            cout << "CXL " << order_id << endl;
        }
        if (cr == CancelResult::Unknown){
            cout << "REJ " << order_id << " UNK" << endl;
        }
    }

    void on_tob(const TopOfBook& tob) override {
        if (tob.best_bid.has_value()){
            cout << "TOB BID " << tob.best_bid.value().price << " " << tob.best_bid.value().qty << endl;
        }
        if (tob.best_ask.has_value()){
            cout << "TOB ASK " << tob.best_ask.value().price << " " << tob.best_ask.value().qty << endl;
        }
    }

    void on_book(const BookSnapshot& bs) override{
        for (auto pl : bs.bids){
            cout << "BOOK BID " << pl.price << " " << pl.qty << endl;
        }
        for (auto pl : bs.asks){
            cout << "BOOK ASK " << pl.price << " " << pl.qty << endl;
        }
    }
};

