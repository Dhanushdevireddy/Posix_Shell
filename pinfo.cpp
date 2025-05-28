#include <iostream>
#include <vector>
#include <cstring>

#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;

void pinfo(vector<string> tokens){

    string proc_status = "T";
    long unsigned int vm_memory = 0;
    

    string statFile = "/proc/"+tokens[1]+"/stat";
    ifstream statStream(statFile);
    string status;

    int ppid, pgrp, session, tty_nr, tpgid;
    pid_t pid;
    string comm;
    getline(statStream, status);
    stringstream ss(status);
    ss >> pid >> comm >> proc_status >> ppid >> pgrp >> session >> tty_nr >> tpgid >> vm_memory;
    
    if (pgrp == tpgid) {
        if(proc_status!="T"){
            proc_status = proc_status + "+";
        }
    }

    string path = "/proc/" + tokens[1] + "/exe";
    char exe_path[1024];
    ssize_t len = readlink(path.c_str(), exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';
    }

    cout<<"pid -- "<<tokens[1]<<"\n";
    cout<<"Process Status -- {"<<proc_status<<"}\n";
    cout<<"memory -- "<<vm_memory<<" {Virtual Memory}\n";
    if(proc_status == "T" || proc_status == "Z"){
        cout<<"Executable Path -- Unable to read the executable path for PID "<<tokens[1]<<"\n";
    }else{
        cout<<"Executable Path -- "<<exe_path<<"\n";
    }

    return;
}