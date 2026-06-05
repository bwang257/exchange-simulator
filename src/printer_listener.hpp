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
        std::cout << "ACK " << order_id << '\n';
    }

    void on_reject(int order_id, RejectReason rr) override{
        if (rr == RejectReason::BAD){
            std::cout << "REJ " << order_id << " BAD" << '\n';
        }
        else if (rr == RejectReason::DUP){
            std::cout << "REJ " << order_id << " DUP" << '\n';
        }
    }

    void on_trade(const Trade& trd) override{
        std::cout << "TRD " << trd.buy_id << " " << trd.sell_id << " " << trd.price << " " << trd.qty << '\n';
    }

    void on_cancel(int order_id, CancelResult cr) override {
        if (cr == CancelResult::Cancelled){
            std::cout << "CXL " << order_id << '\n';
        }
        if (cr == CancelResult::Unknown){
            std::cout << "REJ " << order_id << " UNK" << '\n';
        }
    }

    void on_tob(const TopOfBook& tob) override {
        if (tob.has_bid){
            std::cout << "TOB BID " << tob.bid.price << " " << tob.bid.qty << '\n';
        }

        if (tob.has_ask){
            std::cout << "TOB ASK " << tob.ask.price << " " << tob.ask.qty << '\n';
        }
    }

    void on_book(const BookSnapshot& bs) override{
        for (auto pl : bs.bids){
            std::cout << "BOOK BID " << pl.price << " " << pl.qty << '\n';
        }
        for (auto pl : bs.asks){
            std::cout << "BOOK ASK " << pl.price << " " << pl.qty << '\n';
        }
    }
};

