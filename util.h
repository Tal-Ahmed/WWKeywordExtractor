#ifndef UTIL_H
#define UTIL_H

#include <iostream>

using namespace std;

void error(string err){
    cerr << "ERROR: " << err << endl;
    exit(1);
}

void log(string msg){
    cout << "LOG: " << msg << endl;
}

#endif