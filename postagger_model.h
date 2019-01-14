#ifndef POSTAGGER_MODEL_H
#define POSTAGGER_MODEL_H

#include "mltk/_ctagger.cc"

using namespace std;

class Postagger_Model {
    PerceptronTagger *postagger;
    string train_file;

    void load_pos_tagger();

    public:
        Postagger_Model(string train_file): train_file(train_file), postagger(nullptr) {}

        PerceptronTagger* get_pos_tagger(){
            if (postagger == nullptr){
                load_pos_tagger();
            }

            return postagger;
        }
};

#endif