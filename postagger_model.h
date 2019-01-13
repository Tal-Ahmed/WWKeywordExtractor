#ifndef POSTAGGER_H
#define POSTAGGER_H

#include <string>
#include "maxent/maxent.h"

using namespace std;

class Postagger_Model {
    ME_Model postagger_model;
    bool postagger_model_loaded = false;

    string train_file, test_file;

    public:
        Postagger_Model(string train_file, string test_file):
            train_file(train_file), test_file(test_file) {}

        struct Token {
            string str;
            string pos;
            Token(const string & str, const string & pos): str(str), pos(pos) {}
        };

        void test();
        void train();
        vector<Token> classify_line(const string & str);

        ME_Model get_postagger_model(){
            if (!postagger_model_loaded){
                if (!postagger_model.load_from_file("postagger.model")){
                    train();
                }
            }

            return postagger_model;
        }

};

#endif