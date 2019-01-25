#include <iostream>
#include "util.h"

using namespace std;

void log(string str){
    #ifdef DEBUG
    cout << str << endl;
    #endif
}

void error(string str){
    cerr << str << endl;
    exit(1);
}