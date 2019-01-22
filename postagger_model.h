#ifndef POSTAGGER_MODEL_H
#define POSTAGGER_MODEL_H

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

class PerceptronTagger;
extern unordered_map<string, unordered_map<string, string> > keywords;
extern vector<string> keyword_insertion;

class Postagger_Model {
    PerceptronTagger *postagger;
    map<string, string> *specified_tags;
    vector<pair<string, float> > *bias_weights;
    vector<map<string, vector<pair<string, float> > > > *weights;

    void load_pos_tagger();

    public:
        Postagger_Model(): postagger(nullptr), specified_tags(nullptr), 
            bias_weights(nullptr), weights(nullptr)  {}
        ~Postagger_Model();

        vector<pair<string, string> > tag_sentence(string str);

        PerceptronTagger* get_pos_tagger(){
            if (postagger == nullptr){
                load_pos_tagger();
            }

            return postagger;
        }
};

#endif