/**
printer_listener.hpp
--------------
Defines PrinterListener for outputting events to stdout
 */

#pragma once

#include "events.hpp"
#include "common.hpp"
#include <iostream>

struct PrinterListener : IEventListener {
    
    void on_ack(int order_id) override {
        std::cout << "ACK " << order_id << " ";
    }

    void on_reject(int order_id, RejectReason rr) override{
        if (rr == RejectReason::BAD){
            std::cout << "REJ " << order_id << " BAD ";
        }
        else if (rr == RejectReason::DUP){
            std::cout << "REJ " << order_id << " DUP ";
        }
    }

    void on_trade(const Trade& trd) override{
        std::cout << "TRD " << trd.buy_id << " " << trd.sell_id << " " << trd.price << " " << trd.qty << " ";
    }

    void on_cancel(int order_id, CancelResult cr) override {
        if (cr == CancelResult::Cancelled){
            std::cout << "CXL " << order_id << " ";
        }
        if (cr == CancelResult::Unknown){
            std::cout << "REJ " << order_id << "UNK ";
        }
    }

    void on_tob(const TopOfBook& tob) override {
        if (tob.best_bid.has_value()){
            std::cout << "TOB BID " << tob.best_bid.value().price << " " << tob.best_bid.value().qty << " ";
        }
        if (tob.best_ask.has_value()){
            std::cout << "TOB ASK " << tob.best_ask.value().price << " " << tob.best_ask.value().qty << " ";
        }
    }

    void on_book(const BookSnapshot& bs) override{
        for (auto pl : bs.bids){
            std::cout << "BOOK BID " << pl.price << " " << pl.qty << " ";
        }
        for (auto pl : bs.asks){
            std::cout << "BOOK ASK " << pl.price << " " << pl.qty << " ";
        }
    }
};

