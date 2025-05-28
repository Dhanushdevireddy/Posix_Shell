#include <iostream>
#include <unistd.h>
#include <cstring>

#include "tokenizer.h"

using namespace std;

void exec(string command){

    char* args[64];
    vector<string> temp;
    temp = tokenizer(command, " \t");
    
    unsigned long int i =0;
    for(;i<temp.size();i++){
        args[i] = &temp[i][0];
    }
    args[i] = NULL;

    if (execvp(args[0], args) == -1) {
        cout<<"Command '"<<command<<"' not found.\n";
    }
    
}