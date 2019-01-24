#ifndef KEYWORD_EXTR_MODEL_H
#define KEYWORD_EXTR_MODEL_H

#include <iostream>
#include <vector>
#include "maxent/maxent.h"
#include "postagger_model.h"

using namespace std;

extern unordered_map<string, unordered_map<string, string> > keywords;

class Keyword_Extractor_Model {
    Postagger_Model postagger_model;

    ME_Model extractor_model;
    bool extractor_model_loaded;

    string train_file, test_file;

    void init();

    public:
        Keyword_Extractor_Model(string train_file, string test_file): 
            extractor_model_loaded(false), train_file(train_file), test_file(test_file) {
                init();
            }

        struct ClassifiedToken {
            string word, pos_tag, type;
            ClassifiedToken(const string & w, const string & pos, const string & t):
                word(w), pos_tag(pos), type(t) {}
        };

        struct KeywordToken {
            string word, pos, keywords, types;
            bool is_keyword;
        };

        void test();
        void train();

        vector<Keyword_Extractor_Model::KeywordToken> classify_line(string str);
        ME_Model get_extractor_model(){
            if (!extractor_model_loaded){
                if (extractor_model.load_from_file("extractor.model")){
                    cout << "loaded extractor.model from file" << endl;
                } else {
                    train();
                }
            }

            return extractor_model;
        }

    private:
        ME_Sample generate_sample(vector<ClassifiedToken> & classified_words, int i);
        vector<ClassifiedToken> read_line(const string & str);
};

#endif