/* 
 * File:   main.cpp
 * Author: demiin
 *
 * Created on May 21, 2015, 11:50 AM
 */

#include <cstdlib>
#include <chrono>
#include <ctime>
#include <regex>
#include <string>
#include <iostream>
//#include <boost/lexical_cast.hpp>
#include <fstream>
#include <dirent.h>
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "cfg.h"


using namespace std;
    
void checkPS(string psNameToSearch);
void timerTimeout(size_t interval);
vector<string> getListPS();
void loadCfg();
void error(const char *msg);
void sendMsg(string host, string port, string psName);

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


vector<string> getListPS() {
    vector<string> ps;
    DIR* fdDIR;
    struct dirent *dp;
    fdDIR = opendir("/proc");
    while ((dp =readdir(fdDIR)) != NULL) {
        regex re("(\\d{1,})");
        smatch result;
        string path = string(dp->d_name);

        if (regex_match(path, result, re)) {        
            ifstream f("/proc/" + string(result[1]) + "/comm");
            if (f.is_open()) {
                char* psName = new char[30];
                f.getline(psName, 30);
                ps.push_back(psName);
                //cout << psName << endl;
                f.close();
                delete[] psName;
            }
        }
    }
    
    return ps;
}

void checkPS(string psNameToSearch) {
//    for (;;) {
        vector<string> ps = getListPS();
        for (string &psName: ps) {
            if (psName.find(psNameToSearch) != string::npos ) {
                    //cout << true << endl;
                    sendMsg("192.168.9.26", "31771", psNameToSearch);
                }

            
            }
//    }
}

void timerTimeout(const size_t interval) {
    vector<string> psMon = {"soffice.bin"};
    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    chrono::time_point<chrono::system_clock> timeout = chrono::system_clock::now();;
    size_t elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout-start).count();
    
    for (;;) {
        timeout = std::chrono::system_clock::now();
        elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout-start).count();
        
        if (elapsed_seconds >= interval) {
            start = chrono::system_clock::now();
            thread threadPSsearch(&checkPS, psMon.at(0));
            threadPSsearch.detach();
        }
    }
}






void sendMsg(string host, string port, string psName) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    string buffer;

    portno = atoi(port.c_str());
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("Ошибка открытия сокета");

    server = gethostbyname(host.c_str());
    if (server == NULL) {
        fprintf(stderr,"Хост не найден\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    
    serv_addr.sin_port = htons(portno);
    
    try {
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
            cout << "Ошибка подключения" << endl;
        else {

            n = write(sockfd, psName.c_str(), psName.length());
    
            if (n < 0) 
                 error("Ошибка записи в сокет");

            char buff[256];
            bzero(buff,256);
            n = read(sockfd, buff, 255);
            
            if (n < 0) 
                 error("Ошибка чтения из сокета");
    
            printf("%s\n", buff);
        }
    }
    catch(exception* e) {
        
    }
    close(sockfd);
}



/*
 * 
 */
int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    const size_t sec = 2;
    
    thread threadTimer(&timerTimeout, sec);
    threadTimer.detach();
    for (;;) {}
    

    
    
    return 0;
}