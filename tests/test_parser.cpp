/**
test_parser.cpp
--------------
Implements simple unit tests for parser.cpp
 */

#include "parser.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
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

    // Test parse_commands with valid batch
    string batch = "N 1 B 100 10\nN 2 S 105 5\nP\nC 1\nX\n";
    vector<Command> commands = parse_commands(batch);
    assert(commands.size() == 5);
    assert(commands[0].type == CommandType::New);
    assert(commands[0].order_id == 1);
    assert(commands[0].side == Side::Buy);
    assert(commands[0].price == 100);
    assert(commands[0].qty == 10);
    
    assert(commands[1].type == CommandType::New);
    assert(commands[1].order_id == 2);
    assert(commands[1].side == Side::Sell);
    assert(commands[1].price == 105);
    assert(commands[1].qty == 5);
    
    assert(commands[2].type == CommandType::PrintTopOfBook);
    assert(commands[3].type == CommandType::Cancel);
    assert(commands[3].order_id == 1);
    assert(commands[4].type == CommandType::Exit);

    // Test parse_commands with empty batch
    batch = "";
    commands = parse_commands(batch);
    assert(commands.size() == 0);  // Empty string returns no commands

    // Test parse_commands with multiple lines including invalid
    batch = "N 1 B 100 10\ninvalid\nC 1\nP\n";
    commands = parse_commands(batch);
    assert(commands.size() == 4);
    assert(commands[0].type == CommandType::New);
    assert(commands[1].type == CommandType::Reject);  // Invalid line
    assert(commands[2].type == CommandType::Cancel);
    assert(commands[3].type == CommandType::PrintTopOfBook);

    // Test parse_commands with only newlines
    batch = "\n\n\n";
    commands = parse_commands(batch);
    assert(commands.size() == 3);  // Each empty line becomes a Reject
    for (const auto& cmd : commands) {
        assert(cmd.type == CommandType::Reject);
    }

    // Test parse_commands with mixed valid and invalid commands
    batch = "N 10 B 50 20\nC 10\nN 20 S 60 15\nP\nB\n";
    commands = parse_commands(batch);
    assert(commands.size() == 5);
    assert(commands[0].type == CommandType::New);
    assert(commands[0].order_id == 10);
    assert(commands[1].type == CommandType::Cancel);
    assert(commands[1].order_id == 10);
    assert(commands[2].type == CommandType::New);
    assert(commands[2].order_id == 20);
    assert(commands[3].type == CommandType::PrintTopOfBook);
    assert(commands[4].type == CommandType::PrintFullBook);

    cout << "test_parser: PASS" << endl;
    return 0;


}






