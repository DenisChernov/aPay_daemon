/* 
 * File:   cfg.cpp
 * Author: demiin
 * 
 * Created on May 21, 2015, 12:16 PM
 */

#include "cfg.h"

cfg::cfg() {
    
}


cfg::~cfg() {
}

bool cfg::loadCfg() {
    ifstream f(cfg_filepath);
    string line;
    if (f.is_open()) {
        getline(f, line);
        regex re("([a-zA-Z]*)=([a-zA-Z0-9]*)");
        smatch result;
        if (regex_search(line, result, re)) {
            cout << result[1] << " % " << result[2] <<endl;
        }
        
        f.close();
        return true;
    }
    else
        createNewCfg();
    
    return false;
}

void cfg::createNewCfg() {

}
