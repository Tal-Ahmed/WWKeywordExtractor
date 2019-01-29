#include <iostream>
#include <sstream>
#include "keyword_extr_model.h"
#include "util.h"

#ifdef WASM
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#endif

using namespace std;

int main(int argc, char *argv[]){
    #ifndef WASM
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
    } else if (cmd == "repl"){
        string str;
        while (getline(cin, str)){
            vector<Keyword_Extractor_Model::KeywordToken> keys = keyword_extractor_model.classify_line(str);
            for (int i = 0; i < keys.size(); i += 1){
                cout << keys[i].word << "/" << keys[i].pos << "/" << keys[i].is_keyword << " ";
            }
            cout << endl;
        }
    }
    #endif

    return 0;
}

vector<Keyword_Extractor_Model::KeywordToken> generate_keywords(string str){
    static Keyword_Extractor_Model keyword_extractor_model("data/train.txt", "data/test.txt");
    vector<Keyword_Extractor_Model::KeywordToken> keys = keyword_extractor_model.classify_line(str);
    ostringstream oss;
    for (int i = 0; i < keys.size(); i += 1){
        oss << keys[i].word << "/" << keys[i].pos << "/" << keys[i].is_keyword << " ";
    }
    oss << endl;
    log(oss.str());
    return keys;
}

#ifdef WASM
EMSCRIPTEN_BINDINGS(extractor){
   emscripten::value_object<Keyword_Extractor_Model::KeywordToken>("KeywordToken")
        .field("word", &Keyword_Extractor_Model::KeywordToken::word)
        .field("pos", &Keyword_Extractor_Model::KeywordToken::pos)
        .field("keywords", &Keyword_Extractor_Model::KeywordToken::keywords)
        .field("types", &Keyword_Extractor_Model::KeywordToken::types)
        .field("is_keyword", &Keyword_Extractor_Model::KeywordToken::is_keyword);
   emscripten::register_vector<Keyword_Extractor_Model::KeywordToken>("KeywordTokenVector");
   emscripten::function("generate_keywords", &generate_keywords);
}
#endif