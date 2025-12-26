/*
parser.hpp
-----------------
Defines the Command data model and parser interface.
*/

#pragma once

#include <cstdint>
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

enum class CommandType {
    New,
    Cancel,
    PrintTopOfBook,
    PrintFullBook,
    Exit,
    Reject
};

enum class Side {
    Buy,
    Sell
};

enum class RejectReason {
    BAD
};

struct Command {
    CommandType type;

    std::int64_t order_id = 0;
    
    Side side;
    std::int32_t price = 0;
    std::int32_t qty = 0;

    RejectReason reject_reason; 
};


vector<string> tokenize_input(const string& line);
Command reject_command(int order_id);
Command parse_cancel_command(const vector<string> &tokens);
Command parse_new_command(const vector<string> &tokens);
Command parse_command(const string& line);



