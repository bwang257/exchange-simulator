/**
common.hpp
--------------
Defines common structs across source code
 */

#pragma once

#include <optional>
#include <vector>

enum class Side {
    Buy,
    Sell
};

enum class RejectReason {
    BAD,
    DUP
};

struct Trade {
    int buy_id;
    int sell_id;
    int price;
    int qty;
};

struct PriceLevel {
    int price;
    int qty;
};

struct TopOfBook {
    std::optional<PriceLevel> best_ask;
    std::optional<PriceLevel> best_bid;
};

struct BookSnapshot {
    std::vector<PriceLevel> asks;
    std::vector<PriceLevel> bids;
};