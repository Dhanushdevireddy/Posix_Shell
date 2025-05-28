#include <iostream>
#include <unistd.h>
#include <limits.h> 
#include <pwd.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <fcntl.h> 
#include <sys/wait.h>
#include <csignal>
#include <cstdio>

#include "tokenizer.h"
#include "cd.h"
#include "ls.h"
#include "search.h"
#include "history.h"
#include "pinfo.h"
#include "pipeline.h"
#include "exec.h"
#include "redirect.h"

using namespace std;

pid_t pid = -1;

void sigint(int sig) {
    if(pid > 0){
        kill(pid, SIGINT);
        pid = -1;
        cout<<"\n";
    }
}

void sigstp(int sig) {
    if(pid > 0){
        cout<<"\n";
        cout<<pid;
        kill(pid, SIGTSTP);
        pid = -1;
        cout<<"\n";     
    }
}

int main(){
    //getting hostname
    char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, HOST_NAME_MAX) == -1) {
        perror("Error getting hostname");
        exit(0);
    }

    //getting username
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if(pw==NULL){
        perror("Error getting username");
        exit(0);
    }

    string username;
    username = pw->pw_name;
    
    char curr_wd[PATH_MAX];
    char root_path[PATH_MAX];
    char prev_wd[PATH_MAX] = "NOT SET";

    if(getcwd(root_path, sizeof(root_path))==NULL){
        perror("Error getting root directory");
    }


    while(true){
        
        vector<string> historyList = getHistory(20);

        //printing username, systemname and current directory
        cout<<username<<'@'<<hostname<<':';
        if(getcwd(curr_wd, sizeof(curr_wd))==NULL){
            perror("Error getting current working directory");
        }
        
        if(strcmp(&root_path[0], &curr_wd[0]) == 0){
            cout<<'~';
        }else{
            const char* pos_ptr = strstr(curr_wd, root_path);
            if(pos_ptr){
                cout<<"~";
                size_t pos = pos_ptr - curr_wd;
                for(long unsigned int i =pos+strlen(root_path);curr_wd[i]!='\0';i++){
                    cout<<curr_wd[i];
                }
            }else{
                cout<<curr_wd;
            }
        }
        cout<<"> ";
        
        string command;

        //getting input command and executing ctrl-d command
        if (!getline(cin, command)) {
            cout<<"\nLogout\n";
            break;
        }

        //checking signals for ctrl-z and ctrl-c
        if (signal(SIGINT, sigint) == SIG_ERR){
            perror("Error setting up SIGINT/ctrl-c handler");
            continue;
        }
        if (signal(SIGTSTP, sigstp) == SIG_ERR){
            perror("Error setting up SIGTSTP/ctrl-z handler");
            continue;
        }

        if(command==""){
            continue;
        }

        //adding command to history
        writeToHistory(command);

        //call tokenizer function
        vector<string> commands;
        commands = tokenizer(command,";");

        for(unsigned long int iterate=0;iterate<commands.size();iterate++){

            vector<string> tokens;
            tokens = tokenizer(commands[iterate]," \t");

            //calling pipe function
            bool pipe_flag = false;
            for(unsigned long int i =0;i<tokens.size();i++){
                if(tokens[i]=="|"){
                    pipeline(tokens);
                    pipe_flag = true;
                    break;
                }
            }
            if(pipe_flag){
                continue;
            }
            
            //calling redirect function
            bool redirect_flag = false;
            for(unsigned long int i =0;i<tokens.size();i++){
                if(tokens[i]==">" || tokens[i]==">>" || tokens[i]=="<" ){
                    redirect(tokens);
                    redirect_flag = true;
                    break;
                }
            }
            if(redirect_flag){
                continue;
            }

            //implementing exit command
            if(tokens[0]=="exit"){
                exit(0);
            }
            
            //implementing pwd command
            if(tokens[0]=="pwd"){
                if(getcwd(curr_wd, sizeof(curr_wd))==NULL){
                    perror("Error getting current working directory");
                }
                cout<<curr_wd<<"\n";
                continue;
            }

            //calling cd function
            if(tokens[0]=="cd"){
                cd(tokens, root_path, curr_wd, prev_wd);
                continue;
            }

            //implementing echo command
            if(tokens[0]=="echo"){
                for(long unsigned int i =1; i<tokens.size();i++){
                    cout<<tokens[i]<<" ";
                }
                cout<<"\n";
                continue;
            }

            //calling ls function
            if(tokens[0]=="ls"){
                ls(tokens, root_path);
                continue;
            }

            //calling search function
            if(tokens[0]=="search"){
                search(tokens);
                continue;
            }

            //implementing history command
            if(tokens[0]=="history"){
                vector<string> tempHistoryList;
                if(tokens.size()==1){
                    tempHistoryList = getHistory(10);
                }else{
                    tempHistoryList = getHistory(stoi(tokens[1]));
                }
                for(long unsigned int i =0; i<tempHistoryList.size();i++){
                    cout<<tempHistoryList[i];
                }
                continue;
            }

            //calling pinfo function
            if(tokens[0]=="pinfo"){
                pinfo(tokens);
                continue;
            }

            //calling execvp function
            pid = fork();
            if (pid == -1) {
                cout<<"fork failed";
            }
            if(pid==0){
                exec(commands[iterate]);
            }else{
                if(tokens.size()>1 && tokens[1]== "&"){
                    cout<<pid<<"\n";
                }else{
                    int status;
                    waitpid(pid, &status, WUNTRACED);
                }
            }
        }
    }
    
    return 0;
}
