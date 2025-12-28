/*
parser.hpp
-----------------
Defines the Command data model and parser interface.
*/

#pragma once

#include "common.hpp"
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

enum class CommandType {
    New,
    Cancel,
    PrintTopOfBook,
    PrintFullBook,
    Exit,
    Reject
};

struct Command {
    CommandType type;

    std::int64_t order_id = 0;
    
    Side side = Side::Buy;
    std::int32_t price = 0;
    std::int32_t qty = 0;

    RejectReason reject_reason = RejectReason::BAD; 
};


std::vector<std::string> tokenize_input(const std::string& line);
Command reject_command(int order_id);
Command parse_cancel_command(const std::vector<std::string> &tokens);
Command parse_new_command(const std::vector<std::string> &tokens);
Command parse_command(const std::string& line);



