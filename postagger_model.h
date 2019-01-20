#ifndef POSTAGGER_MODEL_H
#define POSTAGGER_MODEL_H

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class PerceptronTagger;

class Postagger_Model {
    PerceptronTagger *postagger;

    void load_pos_tagger();

    public:
        Postagger_Model(): postagger(nullptr) {}
        ~Postagger_Model(){
            if (postagger != nullptr){
                delete postagger;
            }
        }

        vector<pair<string, string> > tag_sentence(string str);

        PerceptronTagger* get_pos_tagger(){
            if (postagger == nullptr){
                load_pos_tagger();
            }

            return postagger;
        }
};

#endif