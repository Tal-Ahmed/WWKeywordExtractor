#ifndef POSTAGGER_MODEL_H
#define POSTAGGER_MODEL_H

#include <iostream>
#include <sstream>
#include <vector>
#include "mltk/_ctagger.cc"

using namespace std;

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

        vector<pair<string, string> > tag_sentence(string str){            
            vector<string> sentence;
            istringstream iss(str);
            string word;

            while (iss >> word){
                sentence.push_back(word);
            }

            vector<tag_t> tagged_words = postagger->tag_sentence(sentence);

            return tagged_words;
        }

        PerceptronTagger* get_pos_tagger(){
            if (postagger == nullptr){
                load_pos_tagger();
            }

            return postagger;
        }
};

#endif