#include <iostream>
#include <fstream>
#include "matching_engine.hpp"
#include "printer_listener.hpp"
#include "parser.hpp"
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;
using std::istream;
using std::ofstream;
using std::ostream;
using std::getline;
using std::string;

void process_commands(istream& input, ostream& output, MatchingEngine& engine, PrinterListener& printer) {
    string line;
    while (getline(input, line)) {
        if (line == "X") break;
        
        auto cmd = parse_command(line);
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
                return;
            default:
                break;
        }
        output << "\n";
    }
}

int main(int argc, char* argv[]){
    MatchingEngine engine;
    PrinterListener printer;
    engine.add_listener(&printer);

    if (argc > 1){
        ifstream input_file(argv[1]);
        if (!input_file.is_open()){
            cerr << "Could not open input file " << argv[1] << endl;
            return 1;
        }
        // process commands from file
        process_commands(input_file, cout, engine, printer);
        input_file.close();
    }
    else {
        // read line by line from stdin
        process_commands(cin, cout, engine, printer);
    }
    return 0;  
}
