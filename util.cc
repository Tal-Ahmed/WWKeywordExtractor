#include <iostream>
#include "util.h"

using namespace std;

void log(string str){
    cout << str << endl;
}

void error(string str){
    cerr << str << endl;
    exit(1);
}