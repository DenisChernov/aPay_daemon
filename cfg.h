/* 
 * File:   cfg.h
 * Author: demiin
 *
 * Created on May 21, 2015, 12:16 PM
 */

#ifndef CFG_H
#define	CFG_H

#include <string>
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

class cfg {
public:
    cfg();
    virtual ~cfg();
    void setHost(string host) {
        this->host = host;
    }

    string getHost() const {
        return host;
    }

    
    bool loadCfg();
private:
    const string cfg_filepath = "~/.apay/apay.cfg";
    string host;
    
    void createNewCfg();
};

#endif	/* CFG_H */

