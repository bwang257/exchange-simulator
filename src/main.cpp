#include <iostream>
#include <string>
#include "matching_engine.hpp"
#include "printer_listener.hpp"
#include "parser.hpp"

using std::cin;
using std::cout;
using std::endl;


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
