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

// Helper function to process a cancel command
Command parse_cancel_command(const vector<string> &tokens){
    if (tokens.size() != 2) return reject_command();
    try {
        int order_id = stoi(tokens[1]);
        if (order_id <= 0) return reject_command();
        return Command{CommandType::Cancel, order_id};
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

    // Opcode must be a single character (N, C, P, B, or X).
    // Only P, B, and X are handled at this stage (Milestone 1).
    // N and C will be added incrementally in later commits.
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
    
    return 0;
}


