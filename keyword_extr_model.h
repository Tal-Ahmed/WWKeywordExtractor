#ifndef KEYWORD_EXTR_MODEL_H
#define KEYWORD_EXTR_MODEL_H

#include <iostream>
#include <vector>
#include "maxent/maxent.h"

using namespace std;

class Keyword_Extractor_Model {
    PerceptronTagger *tagger;

    ME_Model extractor_model;
    bool extractor_model_loaded;

    string train_file, test_file;

    public:
        Keyword_Extractor_Model(string train_file, string test_file): 
            extractor_model_loaded(false), train_file(train_file), test_file(test_file) {
                tagger = new PerceptronTagger();
            }

        ~Keyword_Extractor_Model(){

        }

        struct ClassifiedToken {
            string word, pos_tag, type;
            ClassifiedToken(const string & w, const string & pos, const string & t):
                word(w), pos_tag(pos), type(t) {}
        };

        void test();
        void train();

        void classify_line(vector<ClassifiedToken> words);
        ME_Model get_extractor_model(){
            if (!extractor_model_loaded){
                if (!extractor_model.load_from_file("extractor.model")){
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