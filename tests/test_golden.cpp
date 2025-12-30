/**
test_golden.cpp
--------------
Implements golden tests for order matching functionality
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include "matching_engine.hpp"
#include "test_listener.hpp" 
#include "parser.hpp"

using std::string;
using std::ifstream;
using std::istringstream;
using std::ostringstream;
using std::getline;
using std::cout;
using std::cerr;
using std::endl;

// Reads a file into a string
string read_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file " << filename << endl;
        return "";
    }
    
    ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool compare_outputs(const string& actual, const string& expected) {
    if (actual == expected) {
        return true;
    }
    
    cerr << "Output mismatch!\n";
    cerr << "Expected:\n" << expected << "\n\n";
    cerr << "Actual:\n" << actual << "\n\n";
    return false;
}

void run_test(const string& input_file, const string& expected_file) {

    // input file
    string input = read_file(input_file);
    if (input.empty()) {
        cerr << "Failed to read input file: " << input_file << endl;
        assert(false);
    }
    
    // output file
    string expected = read_file(expected_file);
    if (expected.empty()) {
        cerr << "Failed to read expected file: " << expected_file << endl;
        assert(false);
    }
    
    // Setup engine with string listener
    MatchingEngine engine;
    TestListener listener;
    engine.add_listener(&listener);
    
    // Process commands
    istringstream input_stream(input);
    string line;
    while (getline(input_stream, line)) {
        if (line == "X") break;
        
        auto cmd = parse_command(line);
        switch (cmd.type) {
            case CommandType::New:
                engine.process_new_order(cmd.order_id, cmd.side, cmd.price, cmd.qty);
                break;
            case CommandType::Reject:
                listener.on_reject(cmd.order_id, cmd.reject_reason);
                break;
            case CommandType::Cancel:
                engine.cancel_order(cmd.order_id);
                break;
            case CommandType::PrintTopOfBook:
                engine.top_of_book();
                break;
            case CommandType::PrintFullBook:
                engine.print_book();
                break;
            case CommandType::Exit:
                return;
            default:
                break;
        }
    }
    
    // Compare outputs
    string actual = listener.get_output();
    assert(compare_outputs(actual, expected));
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Please input: " << argv[0] << " <input_file> <expected_file>" << endl;
        return 1;
    }
    
    run_test(argv[1], argv[2]);
    cout << "Golden test passed!" << endl;
    return 0;
}