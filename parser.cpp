/**
parser.cpp
--------------
Implements parsing of raw input lines into structured Command objects.
Responsible for syntax and semandtic validation.
No I/O or outputs performed. 
 */

#include "parser.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::istringstream;
using std::stoi;
using std::size_t;

using std::invalid_argument;
using std::out_of_range;

// Splits an input line into whitespace-delimited tokens.
vector<string> tokenize_input(const string& line){
    istringstream iss(line);
    vector<string> tokens;
    string token;

    while (iss >> token) tokens.push_back(token);
    return tokens;
}


// Helper function to construct a BAD reject command
// order_id defaults to 0.
Command reject_command(int order_id = 0){
    Command c{CommandType::Reject, order_id};
    c.reject_reason = RejectReason::BAD;
    return c;
}

// Helper function to process a cancel command "C <order_id>"
Command parse_cancel_command(const vector<string> &tokens){
    if (tokens.size() != 2) return reject_command();
    try {
        size_t pos = 0;
        int order_id = stoi(tokens[1], &pos);
        if (order_id <= 0 || pos != tokens[1].size()) return reject_command();
        return Command{CommandType::Cancel, order_id};
    }
    catch (const invalid_argument& e) {
        return reject_command();
    } catch (const out_of_range& e) {
        return reject_command();
    }
}

// Helper function to process a new order command "N <order_id> <side> <price (ticks)> <qty>"
Command parse_new_command(const vector<string> &tokens){
    if (tokens.size() != 5) return reject_command();
    try {
        size_t pos = 0;
        int order_id = stoi(tokens[1], &pos);
        if (order_id <= 0 || pos != tokens[1].size()) return reject_command();

        // side should be "B" (buy) or "S" (sell)
        if (tokens[2].size() != 1 || (tokens[2][0] != 'B' && tokens[2][0] != 'S')) return reject_command(order_id);
        Side side = tokens[2][0] == 'B' ? Side::Buy : Side::Sell;

        pos = 0;
        int price = stoi(tokens[3], &pos);
        if (price <= 0 || pos != tokens[3].size()) return reject_command(order_id);

        pos = 0;
        int qty = stoi(tokens[4], &pos);
        if (qty <= 0 || pos != tokens[4].size()) return reject_command(order_id);

        return Command{CommandType::New, order_id, side, price, qty};
    }
    catch (const invalid_argument& e) {
        return reject_command();
    } catch (const out_of_range& e) {
        return reject_command();
    }
}


// Parses a single input line into a Command.
// This function never throws and always returns a Command.
// Malformed or invalid input results in a Reject(BAD) command.
Command parse_command(const string& line){
    vector<string> tokens = tokenize_input(line);
    if (tokens.size() == 0){
        return reject_command();
    }
    string op = tokens[0];

    // Empty or whitespace-only lines are treated as malformed input.
    if (op.size() != 1){
        return reject_command();
    }

    // op must be a single character (N, C, P, B, or X).
    switch (op[0]){
        case 'P':
            if (tokens.size() == 1) return Command{CommandType::PrintTopOfBook};
            return reject_command();
            break;
        case 'B':
            if (tokens.size() == 1) return Command{CommandType::PrintFullBook};
            return reject_command();
            break;
        case 'X':
            if (tokens.size() == 1) return Command{CommandType::Exit};
            return reject_command();
            break;
        case 'C':
            return parse_cancel_command(tokens);
            break;
        case 'N':
            return parse_new_command(tokens);
            break;
        default:
            return reject_command();
    }
}


// Temporary driver for parser sanity checks.
// This will be replaced once proper tests and engine wiring are added.
int main(){
    Command c = parse_command("");
    if (c.type == CommandType::Reject){
        std::cout << "Empty String Works" << std::endl;
    }
    c = parse_command("B 3243");
    if (c.type == CommandType::Reject){
        std::cout << "B Validation Works" << std::endl;
    }
    c = parse_command("B");
    if (c.type == CommandType::PrintFullBook){
        std::cout << "B works" << std::endl;
    }
    c = parse_command("P");
    if (c.type == CommandType::PrintTopOfBook){
        std::cout << "P Works" << std::endl;
    }
    c = parse_command("X");
    if (c.type == CommandType::Exit) std::cout << "X works" << std::endl;

    c= parse_command("C abc");
    if (c.type == CommandType::Reject){
        std::cout << "C validation works" << std::endl;
    }

    c = parse_command("C 3543");
    if (c.type == CommandType::Cancel){
        std::cout << "C works for " << c.order_id << std::endl;
    }
     
    c = parse_command("N 35");
    if (c.type == CommandType::Reject){
        std::cout << "N validation works" << std::endl;
    }

    c = parse_command("N 1 B 101 10");
    if (c.type == CommandType::New && c.order_id == 1 && c.side == Side::Buy && c.price == 101 && c.qty == 10){
        std::cout << "N works" << std::endl;
    }

    return 0;
}


