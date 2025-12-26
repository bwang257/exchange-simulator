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

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;

// Splits an input line into whitespace-delimited tokens.
// Tokenizes only. 
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
    return Command{CommandType::Reject, order_id};
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
        cout << "B Validation Works" << endl;
    }
    c = parse_command("B");
    if (c.type == CommandType::PrintFullBook){
        cout << "B works" << endl;
    }
    c = parse_command("P");
    if (c.type == CommandType::PrintTopOfBook){
        cout << "P Works" << endl;
    }
    c = parse_command("X");
    if (c.type == CommandType::Exit) cout << "X works" << endl;
    
    return 0;
}











