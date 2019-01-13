#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void error(string err){
    cerr << "ERROR: " << err << endl;
    exit(EXIT_FAILURE);
}

void log(string msg){
    cout << "LOG: " << msg << endl;
}

#endif