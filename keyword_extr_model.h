#ifndef KEYWORD_EXTR_MODEL_H
#define KEYWORD_EXTR_MODEL_H

#include <iostream>
#include <vector>
#include "maxent/maxent.h"
#include "postagger_model.h"

using namespace std;

class Keyword_Extractor_Model {
    Postagger_Model postagger_model{"maxent/sample_data/train.pos", "maxent/sample_data/dev.pos"};

    ME_Model extractor_model;
    bool extractor_model_loaded = false;

    string train_file, test_file;

    public:
        Keyword_Extractor_Model(string train_file, string test_file): 
            train_file(train_file), test_file(test_file) {}

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

};



#endif