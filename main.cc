#include <iostream>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include "keyword_extr_model.h"

using namespace std;

int main(int argc, char *argv[]){
    return 0;
}

vector<Keyword_Extractor_Model::KeywordToken> generate_keywords(string str){
    static Keyword_Extractor_Model keyword_extractor_model("data/train.txt", "data/test.txt");
    vector<Keyword_Extractor_Model::KeywordToken> keys = keyword_extractor_model.classify_line(str);
    for (int i = 0; i < keys.size(); i += 1){
        cout << keys[i].word << "/" << keys[i].pos << "/" << keys[i].is_keyword << " ";
    }
    cout << endl;
    return keys;
}

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