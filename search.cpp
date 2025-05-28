#include <iostream>
#include <vector>
#include <cstring>
#include <dirent.h>

using namespace std;

void search(vector<string> tokens){
    DIR* dir_ptr = opendir(".");
    if(dir_ptr==NULL){
        perror("Error opening directory");
        return;
    }
    struct dirent* content;
    while((content = readdir(dir_ptr))!=NULL){
        if(strcmp(content->d_name,&tokens[1][0])==0){
            cout<<"True\n";
            closedir(dir_ptr);
            return;
        }
    }
    closedir(dir_ptr);
    cout<<"False\n";
    return;
}