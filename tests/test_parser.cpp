/**
test_parser.cpp
--------------
Implements simple unit tests for parser.cpp
 */

#include "parser.hpp"
#include <cassert>
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

int main(){

    // valid inputs
    string line = "P";
    Command c = parse_command(line);
    assert(c.type == CommandType::PrintTopOfBook);

    line = "B";
    c = parse_command(line);
    assert(c.type == CommandType::PrintFullBook);

    line = "X";
    c = parse_command(line);
    assert(c.type == CommandType::Exit);

    line = "C 12";
    c = parse_command(line);
    assert(c.type == CommandType::Cancel);
    assert(c.order_id == 12);

    line = "N 1 B 101 10";
    c = parse_command(line);
    assert(c.type == CommandType::New);
    assert(c.order_id == 1);
    assert(c.side == Side::Buy);
    assert(c.price == 101);
    assert(c.qty == 10);

    line = "N 2 S 105 5";
    c = parse_command(line);
    assert(c.type == CommandType::New);
    assert(c.order_id == 2);
    assert(c.side == Side::Sell);
    assert(c.price == 105);
    assert(c.qty == 5);


    // invalid inputs
    line = "";
    c = parse_command(line);
    assert(c.type == CommandType::Reject);
    assert(c.reject_reason == RejectReason::BAD);

    line = " ";
    c = parse_command(line);
    assert(c.type == CommandType::Reject);
    assert(c.reject_reason == RejectReason::BAD);

    line = "P extra";
    c = parse_command(line);
    assert(c.type == CommandType::Reject);
    assert(c.reject_reason == RejectReason::BAD);

    line = "C 12abc";
    c = parse_command(line);
    assert(c.type == CommandType::Reject);
    assert(c.reject_reason == RejectReason::BAD);

    line = "C 0";
    c = parse_command(line);
    assert(c.type == CommandType::Reject);
    assert(c.reject_reason == RejectReason::BAD);

    line = "C -1";
    c = parse_command(line);
    assert(c.type == CommandType::Reject);
    assert(c.reject_reason == RejectReason::BAD);

    line = "N 1 X 101 10";
    c = parse_command(line);
    assert(c.type == CommandType::Reject);
    assert(c.reject_reason == RejectReason::BAD);

    line = "N 1 B 0 10";
    c = parse_command(line);
    assert(c.type == CommandType::Reject);
    assert(c.reject_reason == RejectReason::BAD);

    line = "N 1 B 101 0";
    c = parse_command(line);
    assert(c.type == CommandType::Reject);
    assert(c.reject_reason == RejectReason::BAD);

    line = "N 1 B 101 10abc";
    c = parse_command(line);
    assert(c.type == CommandType::Reject);
    assert(c.reject_reason == RejectReason::BAD);

    cout << "test_parser: PASS" << endl;
    return 0;


}






