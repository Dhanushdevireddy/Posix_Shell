#include <iostream>
#include <vector>
#include <cstring>
#include <sys/wait.h>

#include "exec.h"

using namespace std;

void pipeline(vector<string> tokens){

    vector<string> commands;
    string temp;
    for(unsigned long int i =0;i<tokens.size();i++){
        if(tokens[i]!="|"){
            temp = temp + " " + tokens[i];
        }else{
            commands.push_back(temp);
            temp = "";
        }
    }
    commands.push_back(temp);

    int pipefd[2 * (commands.size() - 1)]; 

    for (unsigned long int i = 0; i < commands.size() - 1; i++) {
        if (pipe(pipefd + i*2) == -1) {
            cout<<"pipe failed";
        }
    }

    for (unsigned long int i = 0; i < commands.size(); i++) {
        pid_t pid = fork();
        if (pid == -1) {
            cout<<"fork failed";
        }

        if (pid == 0) { 
            if (i > 0) {
                if(dup2(pipefd[(i-1)*2], STDIN_FILENO)==-1){
                    perror("Error duplicating file descriptor");
                    return;
                }
            }
            if (i < commands.size() - 1) {
                if(dup2(pipefd[i*2 + 1], STDOUT_FILENO)==-1){
                    perror("Error duplicating file descriptor");
                    return;
                }
            }

            for (unsigned long int j = 0; j < 2 * (commands.size() - 1); j++) {
                close(pipefd[j]);
            }

            exec(commands[i]);
            
        }
    }

    for (unsigned long int i = 0; i < 2 * (commands.size() - 1); i++) {
        close(pipefd[i]);
    }

    for (unsigned long int i = 0; i < commands.size(); i++) {
        wait(NULL);
    }

    return;
}