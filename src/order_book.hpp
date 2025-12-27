#include <map>
#include <functional>
#include <optional>
#include "parser.hpp"
 

struct PriceLevel {
    int price;
    int qty;
};

struct TopOfBook {
    std::optional<PriceLevel> best_ask;
    std::optional<PriceLevel> best_bid;

};


class OrderBook {
private:
    std::map<int, int> asks;
    std::map<int, int, std::greater<int>> bids;

public: 
    void add_limit(int order_id, Side side, int price, int qty);
    TopOfBook top_of_book() const;
};
