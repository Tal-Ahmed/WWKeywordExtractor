#include <iostream>
#include "keyword_extr_model.h"

using namespace std;

int main(int argc, char *argv[]){
    if (argc != 2){
        cerr << "Usage: " << argv[0] << " [train/test] " << endl;
        return 1;
    }

    Keyword_Extractor_Model keyword_extractor_model("data/train.txt", "data/test.txt");

    string cmd(argv[1]);
    if (cmd == "train"){
        keyword_extractor_model.train();
    } else if (cmd == "test"){
        keyword_extractor_model.test();
    }

    return 0;
}