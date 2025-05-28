#include <iostream>
#include <vector>
#include <cstring>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

void ls(vector<string> tokens, char* root_path){
    if(tokens.size()==1){
        DIR* dir_ptr = opendir(".");
        if(dir_ptr==NULL){
            perror("Error opening the directory");
            return;
        }
        struct dirent* content;
        while((content = readdir(dir_ptr))!=NULL){
            if(content->d_name[0] != '.'){
                cout<<content->d_name<<"\n";
            }
        }
        closedir(dir_ptr);
        return;
    }

    if(tokens[1]=="."){
        DIR* dir_ptr = opendir(".");
        if(dir_ptr==NULL){
            perror("Error opening the directory");
            return;
        }
        struct dirent* content;
        while((content = readdir(dir_ptr))!=NULL){
            if(content->d_name[0] != '.'){
                cout<<content->d_name<<"\n";
            }
        }
        closedir(dir_ptr);
        return;
    }

    if(tokens[1]==".."){
        DIR* dir_ptr = opendir("..");
        if(dir_ptr==NULL){
            perror("Error opening the directory");
            return;
        }
        struct dirent* content;
        while((content = readdir(dir_ptr))!=NULL){
            if(content->d_name[0] != '.'){
                cout<<content->d_name<<"\n";
            }
        }
        closedir(dir_ptr);
        return;
    }

    if(tokens.size()==2 && tokens[1]=="-a"){
        DIR* dir_ptr = opendir(".");
        if(dir_ptr==NULL){
            perror("Error opening the directory");
            return;
        }
        struct dirent* content;
        while((content = readdir(dir_ptr))!=NULL){
            cout<<content->d_name<<"\n";
        }
        closedir(dir_ptr);
        return;
    }

    if(tokens[1]=="~"){
        DIR* dir_ptr = opendir(root_path);
        if(dir_ptr==NULL){
            perror("Error opening the directory");
            return;
        }
        struct dirent* content;
        while((content = readdir(dir_ptr))!=NULL){
            if(content->d_name[0] != '.'){
                cout<<content->d_name<<"\n";
            }
        }
        closedir(dir_ptr);
        return;
    }

    if((tokens.size()==2 && ( tokens[1]=="-l" ||tokens[1]=="-al" || tokens[1]=="-la")) || (tokens.size()==3 && (( tokens[1] == "-a" && tokens[2] == "-l") || ( tokens[1] == "-l" && tokens[2] == "-a")))){
        DIR* dir_ptr = opendir(".");
        if(dir_ptr==NULL){
            perror("Error opening the directory");
            return;
        }

        struct dirent* content;
        while ((content = readdir(dir_ptr)) != NULL) {
            struct stat file_stat;
            if (stat(content->d_name, &file_stat) == -1) {
                //cout<<"Error reading directory.";
                continue;
            }

            if((tokens.size()==2 && tokens[1]=="-l") && content->d_name[0] == '.'){
                continue;
            }

            cout << ((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
            cout << ((file_stat.st_mode & S_IRUSR) ? "r" : "-");
            cout << ((file_stat.st_mode & S_IWUSR) ? "w" : "-");
            cout << ((file_stat.st_mode & S_IXUSR) ? "x" : "-");
            cout << ((file_stat.st_mode & S_IRGRP) ? "r" : "-");
            cout << ((file_stat.st_mode & S_IWGRP) ? "w" : "-");
            cout << ((file_stat.st_mode & S_IXGRP) ? "x" : "-");
            cout << ((file_stat.st_mode & S_IROTH) ? "r" : "-");
            cout << ((file_stat.st_mode & S_IWOTH) ? "w" : "-");
            cout << ((file_stat.st_mode & S_IXOTH) ? "x" : "-");

            cout << " " << file_stat.st_nlink;

            struct passwd* pwd = getpwuid(file_stat.st_uid);
            struct group* grp = getgrgid(file_stat.st_gid);
            cout << " " << (pwd ? pwd->pw_name : "UNKNOWN");
            cout << " " << (grp ? grp->gr_name : "UNKNOWN");

            cout << " " << setw(8) << file_stat.st_size;

            char time_buf[80];
            struct tm* time_info = localtime(&file_stat.st_mtime);
            strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", time_info);
            cout << " " << time_buf;

            cout << " " << content->d_name << "\n";
        }
        closedir(dir_ptr);

        return;
    }

    bool flag_for_a = false;
    bool flag_for_l = false;

    for(unsigned long int i =0;i<tokens.size();i++){
        if(tokens[i] == "-a"){
            flag_for_a = true;
        }
        if(tokens[i]=="-l"){
            flag_for_l = true;
        }
        if(tokens[i]=="-la" || tokens[i]=="-al"){
            flag_for_l = true;
            flag_for_a = true;
        }
    }

    if(flag_for_l){
        for(unsigned long int i =1;i<tokens.size();i++){
            if(tokens[i]!="-l" && tokens[i]!="-a" && tokens[i]!="-la" && tokens[i]!="-al"){

                bool file_flag = false;
                for(unsigned long int j =0;j<tokens[i].size();j++){
                    if(tokens[i][j]=='.'){
                        file_flag = true;
                        break;
                    }
                }
                if(file_flag){
                    DIR* dir_ptr = opendir(".");
                    if (dir_ptr == NULL) {
                        perror("Error opening directory");
                        return;
                    }

                    struct dirent* content;
                    while ((content = readdir(dir_ptr)) != NULL) {
                        struct stat file_stat;
                        if (stat(content->d_name, &file_stat) == -1) {
                            //cout<<"Error reading directory.";
                            continue;
                        }
                        if(strcmp(content->d_name,&tokens[i][0])!=0){
                            continue;
                        }

                        if( !flag_for_a && content->d_name[0] == '.'){
                            continue;
                        }

                        cout << ((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
                        cout << ((file_stat.st_mode & S_IRUSR) ? "r" : "-");
                        cout << ((file_stat.st_mode & S_IWUSR) ? "w" : "-");
                        cout << ((file_stat.st_mode & S_IXUSR) ? "x" : "-");
                        cout << ((file_stat.st_mode & S_IRGRP) ? "r" : "-");
                        cout << ((file_stat.st_mode & S_IWGRP) ? "w" : "-");
                        cout << ((file_stat.st_mode & S_IXGRP) ? "x" : "-");
                        cout << ((file_stat.st_mode & S_IROTH) ? "r" : "-");
                        cout << ((file_stat.st_mode & S_IWOTH) ? "w" : "-");
                        cout << ((file_stat.st_mode & S_IXOTH) ? "x" : "-");

                        cout << " " << file_stat.st_nlink;

                        struct passwd* pwd = getpwuid(file_stat.st_uid);
                        struct group* grp = getgrgid(file_stat.st_gid);
                        cout << " " << (pwd ? pwd->pw_name : "UNKNOWN");
                        cout << " " << (grp ? grp->gr_name : "UNKNOWN");

                        cout << " " << setw(8) << file_stat.st_size;

                        char time_buf[80];
                        struct tm* time_info = localtime(&file_stat.st_mtime);
                        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", time_info);
                        cout << " " << time_buf;

                        cout << " " << content->d_name << "\n";
                        break;
                    }
                    closedir(dir_ptr);
                }else{
                    cout<<tokens[i]<<":\n";
                    DIR* dir_ptr = opendir(&tokens[i][0]);
                    if (dir_ptr == NULL) {
                        perror("Error opening directory");
                        return;
                    }

                    struct dirent* content;
                    while ((content = readdir(dir_ptr)) != NULL) {
                        struct stat file_stat;
                        if (stat(content->d_name, &file_stat) == -1) {
                            continue;
                        }

                        if( !flag_for_a && content->d_name[0] == '.'){
                            continue;
                        }

                        cout << ((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
                        cout << ((file_stat.st_mode & S_IRUSR) ? "r" : "-");
                        cout << ((file_stat.st_mode & S_IWUSR) ? "w" : "-");
                        cout << ((file_stat.st_mode & S_IXUSR) ? "x" : "-");
                        cout << ((file_stat.st_mode & S_IRGRP) ? "r" : "-");
                        cout << ((file_stat.st_mode & S_IWGRP) ? "w" : "-");
                        cout << ((file_stat.st_mode & S_IXGRP) ? "x" : "-");
                        cout << ((file_stat.st_mode & S_IROTH) ? "r" : "-");
                        cout << ((file_stat.st_mode & S_IWOTH) ? "w" : "-");
                        cout << ((file_stat.st_mode & S_IXOTH) ? "x" : "-");

                        cout << " " << file_stat.st_nlink;

                        struct passwd* pwd = getpwuid(file_stat.st_uid);
                        struct group* grp = getgrgid(file_stat.st_gid);
                        cout << " " << (pwd ? pwd->pw_name : "UNKNOWN");
                        cout << " " << (grp ? grp->gr_name : "UNKNOWN");

                        cout << " " << setw(8) << file_stat.st_size;

                        char time_buf[80];
                        struct tm* time_info = localtime(&file_stat.st_mtime);
                        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", time_info);
                        cout << " " << time_buf;

                        cout << " " << content->d_name << "\n";
                    }
                    closedir(dir_ptr);
                }
            }
        }

        return;
    }else{
        for(unsigned long int i =1;i<tokens.size();i++){

            for(unsigned long int j =0;j<tokens[i].size();j++){
                if(tokens[i][j]=='.'){
                    cout<<tokens[i]<<"\n";
                    return;
                }
            }

            cout<<tokens[i]<<":\n";
            DIR* dir_ptr = opendir(&tokens[i][0]);
            if (dir_ptr == NULL) {
                perror("Error opening directory");
                return;
            }
            struct dirent* content;
            while((content = readdir(dir_ptr))!=NULL){
                if(flag_for_a){
                    cout<<content->d_name<<"\n";
                }else{
                    if(content->d_name[0]!='.'){
                        cout<<content->d_name<<"\n";
                    }
                }
                
                
            }
            closedir(dir_ptr);
            return;
        }
        return;
    }

    cout<<"Please give proper arguments!!";

    return;
}