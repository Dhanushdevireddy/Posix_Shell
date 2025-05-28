#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>

using namespace std;

void cd(vector<string> tokens, char* root_path, char* curr_wd, char* prev_wd){
    if(tokens.size()==1){
        if(chdir(root_path)==-1){
            perror("Error changing directory");
            return;
        }
        strcpy(prev_wd,curr_wd);
        return;
    }

    if(tokens[1]=="."){
        strcpy(prev_wd, curr_wd);
        return;
    }

    if(tokens[1]==".."){
        if(chdir(&tokens[1][0])==-1){
            perror("Error changing directory");
            return;
        }
        strcpy(prev_wd,curr_wd);
        return;
    }

    if(tokens[1]=="-"){
        if(strcmp(prev_wd, "NOT SET") == 0){
            cout<<"bash: cd: OLDPWD not set\n";
            return;
        }
        if(chdir(prev_wd)==-1){
            perror("Error changing directory");
            return;
        }
        strcpy(prev_wd,curr_wd);
        return;
    }
    
    if(tokens[1]=="~"){
        if(chdir(root_path)==-1){
            perror("Error changing directory");
            return;
        }
        strcpy(prev_wd,curr_wd);
        return;
    }

    if(chdir(&tokens[1][0])==0){
        strcpy(prev_wd,curr_wd);
        return;
    }else{
        cout<<"bash: cd: "<<tokens[1]<<": No such file or directory\n";
    }

}