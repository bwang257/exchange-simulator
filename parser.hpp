/*
parser.hpp
-----------------
Defines the Command data model and parser interface.
*/

#pragma once

#include <cstdint>

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




