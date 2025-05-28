#include <iostream>
#include <vector>
#include <cstring>  

using namespace std;

vector<string> tokenizer(string command, const char* delimiters){
    vector<string> tokens;

    char temp[command.size()];
    strcpy(temp,command.c_str());

    char* token = strtok(temp, delimiters);

    while (token != nullptr) {
        tokens.push_back(token);
        token = strtok(nullptr, delimiters);
    }
    
    return tokens;
}