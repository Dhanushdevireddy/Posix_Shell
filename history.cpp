#include <iostream>
#include <vector>
#include <cstring>
#include <fcntl.h> 
#include <unistd.h>
#include <cstdio> 
#include <termios.h>
#include <limits.h>

using namespace std;

vector<string> getHistory(int num){
    vector<string> historyList;
    vector<string> tempHistory;

    
    FILE *file = fopen("/media/dhanush/Partition/MTech/1st_Semester/CS3.304_AOS/Assignments/Assignment-2/2024201028_Assignment2/history.txt", "r"); 

    if (file == nullptr) {
        cout<< "Error opening file 'history.txt'\n";
        return {};
    }

    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file)) {
        tempHistory.push_back(string(buffer)); 
    }
    fclose(file);

    file = fopen("/media/dhanush/Partition/MTech/1st_Semester/CS3.304_AOS/Assignments/Assignment-2/2024201028_Assignment2/history.txt", "w"); 

    if (file == nullptr) {
        cout<< "Error opening file 'history.txt'\n";
        return {};
    }

    if(tempHistory.size()>=20){
        for(long unsigned int i= tempHistory.size()-20;i<tempHistory.size();i++){
            fputs(&tempHistory[i][0], file);
        }
    }else{
        for(long unsigned int i= 0;i<tempHistory.size();i++){
            fputs(&tempHistory[i][0], file);
        }
    }

    fclose(file);

    long unsigned int typeCastedNum = static_cast<long unsigned int>(num);

    if(typeCastedNum>=tempHistory.size()){
        return tempHistory;
    }else{
        for(long unsigned int i =tempHistory.size()-num;i<tempHistory.size();i++){
            historyList.push_back(tempHistory[i]);
        }
    }

    return historyList;
}

void writeToHistory(string command){
    const char *historyFile = "/media/dhanush/Partition/MTech/1st_Semester/CS3.304_AOS/Assignments/Assignment-2/2024201028_Assignment2/history.txt";
    int file = open(historyFile, O_WRONLY | O_APPEND);
    if(file==-1){
        perror("Error opening history.txt");
        return;
    }
    string temp = command + "\n";
    ssize_t bytes_written = write(file, &temp[0], strlen(&temp[0]));
    if (bytes_written == -1) {
        perror("Error writing to history.txt");
        close(file);  
        return;  
    }
    close(file);
    return;
}
