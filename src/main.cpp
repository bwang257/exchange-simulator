#include <iostream>
#include <string>
#include "matching_engine.hpp"
#include "events.hpp"
#include "parser.hpp"

using std::cin;
using std::cout;
using std::endl;

struct PrinterListener : IEventListener {
    
    void on_ack(int order_id) override {
        cout << "ACK " << order_id << " ";
    }

    void on_reject(int order_id, RejectReason rr) override{
        if (rr == RejectReason::BAD){
            cout << "REJ " << order_id << " BAD ";
        }
        else if (rr == RejectReason::DUP){
            cout << "REJ " << order_id << " DUP ";
        }
    }

    void on_trade(const Trade& trd) override{
        cout << "TRD " << trd.buy_id << " " << trd.sell_id << " " << trd.price << " " << trd.qty << " ";
    }

    void on_cancel(int order_id, CancelResult cr) override {
        if (cr == CancelResult::Cancelled){
            cout << "CXL " << order_id << " ";
        }
        if (cr == CancelResult::Unknown){
            cout << "REJ " << order_id << "UNK ";
        }
    }

    void on_tob(const TopOfBook& tob) override {
        if (tob.best_bid.has_value()){
            cout << "TOB BID " << tob.best_bid.value().price << " " << tob.best_bid.value().qty << " ";
        }
        if (tob.best_ask.has_value()){
            cout << "TOB ASK " << tob.best_ask.value().price << " " << tob.best_ask.value().qty << " ";
        }
    }

    void on_book(const BookSnapshot& bs) override{
        for (auto pl : bs.bids){
            cout << "BOOK BID " << pl.price << " " << pl.qty << " ";
        }
        for (auto pl : bs.asks){
            cout << "BOOK ASK " << pl.price << " " << pl.qty << " ";
        }
    }
};


int main(){
    MatchingEngine engine;
    PrinterListener printer;
    engine.add_listener(&printer);

    std::string input;
    do {
        std::getline(cin, input);
        auto cmd = parse_command(input);  
        switch (cmd.type) {
            case CommandType::New:
                engine.process_new_order(cmd.order_id, cmd.side, cmd.price, cmd.qty);
                break;
            case CommandType::Reject:
                printer.on_reject(cmd.order_id, cmd.reject_reason);
                break;
            case CommandType::Cancel:
                engine.cancel_order(cmd.order_id);
                break;
            case CommandType::PrintTopOfBook:
                engine.top_of_book();
                break;
            case CommandType::PrintFullBook:
                engine.print_book();
                break;
            case CommandType::Exit:
                break;
            default:
                break;
        } 
        cout << "\n"; 
    } while (input != "X");
    return 0;    
}
