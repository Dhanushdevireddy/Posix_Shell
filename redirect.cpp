#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "exec.h"

using namespace std;

void redirect(vector<string> tokens){
    string command = "";
    string input_file = "";
    string output_file = "";
    bool read = false;
    bool write = false;
    bool append = false;
    unsigned long int i = 0;
    for(; i<tokens.size();i++){
        if(tokens[i]!=">" && tokens[i]!= "<" && tokens[i]!=">>"){
            command = command +" "+ tokens[i];
        }else{
            break;
        }
    }
    if(tokens[i]==">"){
        write = true;
    }
    if(tokens[i]==">>"){
        append = true;
    }
    if(tokens[i]=="<"){
        read = true;
    }
    i++;
    if(read){
        input_file = tokens[i];
    }else{
        output_file = tokens[i];
    }
    i++;
    if(i!=tokens.size()){
        if(tokens[i]==">"){
            write = true;
        }
        if(tokens[i]==">>"){
            append = true;
        }
        if(tokens[i]=="<"){
            read = true;
        }
        i++;
        if(read){
            input_file = tokens[i];
        }else{
            output_file = tokens[i];
        }
        i++;
    }

    pid_t pid = fork();
    if (pid == -1) {
        cout<<"fork failed";
        return;
    }

    if(pid==0){
        int input_fd=-1;
        if(read){
            input_fd = open(&input_file[0], O_RDONLY);
            if (input_fd < 0) {
                perror("Error opening input file");
                return;
            }
            if(dup2(input_fd, STDIN_FILENO)==-1){
                perror("Error duplicating file descriptor");
                return;
            }
        }

        int output_fd=-1;
        if(write){
            output_fd = open(&output_file[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd < 0) {
                perror("Error creating/opening output file");
                return;
            }
            if(dup2(output_fd, STDOUT_FILENO)==-1){
                perror("Error duplicating file descriptor");
                return;
            }
        }
        if(append){
            output_fd = open(&output_file[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (output_fd < 0) {
                perror("Error creating/opening output file");
                return;
            }
            if(dup2(output_fd, STDOUT_FILENO)==-1){
                perror("Error duplicating file descriptor");
                return;
            }
        }

        exec(command);
        if(read){
            close(input_fd);
        }
        if(write || append){
            close(output_fd);
        }
    }else{
        int status;
        waitpid(pid, &status, 0);
    }

    return;
}