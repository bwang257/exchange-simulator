/**
matching_engine.hpp
--------------
Defines the Matching Engine interface and Trade struct
 */

#pragma once

#include "order_book.hpp"
#include <vector>
#include "events.hpp"

struct NewOrderResponse {
    bool accepted;
    std::optional<RejectReason> reject_reason;
    std::vector<Trade> trades;
};

class MatchingEngine{
private:
    OrderBook ob;
    std::vector<IEventListener*> listeners;
    std::vector<Trade> order_match_buy(int incoming_id, int incoming_price, int& remaining_qty);
    std::vector<Trade> order_match_sell(int incoming_id, int incoming_price, int& remaining_qty);

public:
    void add_listener(IEventListener* l);
    NewOrderResponse process_new_order(int order_id, Side side, int price, int qty);
    TopOfBook top_of_book() const;
    BookSnapshot print_book() const;
    CancelResult cancel_order(int order_id);
};