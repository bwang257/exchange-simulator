/**
test_listener.hpp
--------------
Defines TestListener for outputting events to a stringstream
 */

#pragma once
#include "events.hpp"
#include <sstream>
#include <string>

using std::ostringstream;
using std::endl;
using std::string;


class TestListener : public IEventListener {
private:
    ostringstream output;
    
public:
    void on_ack(int order_id) override {
        output << "ACK " << order_id << endl;
    }
    
    void on_reject(int order_id, RejectReason rr) override {
        if (rr == RejectReason::BAD) {
            output << "REJ " << order_id << " BAD" << endl;
        } else if (rr == RejectReason::DUP) {
            output << "REJ " << order_id << " DUP" << endl;
        }
    }
    
    void on_trade(const Trade& trd) override {
        output << "TRD " << trd.buy_id << " " << trd.sell_id << " " << trd.price << " " << trd.qty << endl;
    }
    
    void on_cancel(int order_id, CancelResult cr) override {
        if (cr == CancelResult::Cancelled) {
            output << "CXL " << order_id << endl;
        } else if (cr == CancelResult::Unknown) {
            output << "REJ " << order_id << " UNK" << endl;
        }
    }
    
    void on_tob(const TopOfBook& tob) override {
        if (tob.best_bid.has_value()) {
            output << "TOB BID " << tob.best_bid.value().price << " " << tob.best_bid.value().qty << endl;
        }
        if (tob.best_ask.has_value()) {
            output << "TOB ASK " << tob.best_ask.value().price << " " << tob.best_ask.value().qty << endl;
        }
    }
    
    void on_book(const BookSnapshot& bs) override {
        for (const auto& pl : bs.bids) {
            output << "BOOK BID " << pl.price << " " << pl.qty << endl;
        }
        for (const auto& pl : bs.asks) {
            output << "BOOK ASK " << pl.price << " " << pl.qty << endl;
        }
    }
    
    string get_output() const {
        return output.str();
    }
    
    void clear() {
        output.str("");
        output.clear();
    }
};