/* 
 * File:   main.cpp
 * Author: demiin
 *
 * Created on May 21, 2015, 11:50 AM
 */

#include <cstdlib>
#include <regex>
#include <string>
#include <iostream>
//#include <boost/lexical_cast.hpp>
#include <fstream>
#include <dirent.h>
#include <thread>

#include "cfg.h"


using namespace std;

void checkPS(string psNameToSearch);
vector<string> getListPS();
void loadCfg();

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
    for (;;) {
        vector<string> ps = getListPS();
        for (string psName: ps) {
            if (psName.find(psNameToSearch) != string::npos ) {
                    cout << true << endl;
                }
            }
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    cfg config;
    if (!config.loadCfg()) {
        cout << "bad" << endl;
    }
        
    
    
/*    vector<string> psMon = {"soffice.bin"};
    thread threadPSsearch(&checkPS, psMon.at(0));
    threadPSsearch.detach();
    
    for (;;) {} */
    return 0;
}

