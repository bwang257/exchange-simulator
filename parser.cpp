#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "parser.hpp"

using std::vector;
using std::string;
using std::istringstream;

vector<string> tokenize_input(const string& line){
    istringstream iss(line);
    vector<string> tokens;
    string token;

    while (iss >> token) tokens.push_back(token);
    return tokens;
}












