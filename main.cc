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

    string str;
    while (getline(cin, str)){
        vector<Keyword_Extractor_Model::KeywordToken> keys = keyword_extractor_model.classify_line(str);
        for (int i = 0; i < keys.size(); i += 1){
            cout << keys[i].word << "/" << keys[i].pos << "/" << keys[i].is_keyword << " ";
        }
        cout << endl;
    }

    return 0;
}