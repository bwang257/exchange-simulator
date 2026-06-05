/**
common.hpp
--------------
Defines common structs across source code
 */

#pragma once

#include <optional>
#include <vector>
#include <array>
#include <cstdint>

using Price = int64_t;
using Quantity = int64_t;

enum class Side : uint8_t {
    Buy = 1,
    Sell = 2
};
static_assert(sizeof(Side) == 1);

enum class RejectReason : uint8_t {
    BAD,
    DUP
};
static_assert(sizeof(RejectReason) == 1);

struct Trade {
    // Todo: Add ticker
    int buy_id;
    int sell_id;
    int price;
    int qty;
};

struct PriceLevel {
    Price price;
    Quantity qty;
};
static_assert(sizeof(PriceLevel) == 16);


struct TopOfBook {
    PriceLevel ask;
    PriceLevel bid;
    bool has_bid{false};
    bool has_ask{false};
    uint8_t _pad[14];
};
static_assert(sizeof(TopOfBook) == 48);


// something to transition into (TODO: Evaluate)
template <std::size_t N >
struct FixedBookSnapshot { 
    std::array<PriceLevel, N> asks;
    std::array<PriceLevel, N> bids;
    std::size_t ask_cnt = 0;
    std::size_t bid_cnt = 0;
};


struct BookSnapshot {
    std::vector<PriceLevel> asks;
    std::vector<PriceLevel> bids;
};

