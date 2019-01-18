#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "util.h"
#include "postagger_model.h"
#include "mltk/_ctagger.cc"

using namespace std;

void Postagger_Model::load_pos_tagger(){
    cout << "start loading pos tagger" << endl;

    tagmap_in_t specified_tags;
    ifstream st_fs("data/specified_tags.txt");
    string k, v;

    while (st_fs >> k >> v){
        specified_tags[k] = v;
    }

    cout << "finish reading data/specified_tags.txt" << endl;

    class_weights_in_t bias_weights;
    ifstream bw_fs("data/bias_weights.txt");
    string k2;
    float v2;

    while (bw_fs >> k2 >> v2){
        bias_weights.push_back(make_pair(k2, v2));
    }

    cout << "finish reading data/bias_weights.txt" << endl;

    weights_in_t weights;
    ifstream w_fs("data/weights.txt");
    string k3, v3_1;
    float v3_2;

    while (w_fs.peek() != EOF){
        map<string, class_weights_in_t> m;
        string line;
        while (getline(w_fs, line)){
            istringstream iss(line);
            iss >> k3;

            if (k3 == "####"){
                weights.push_back(m);
                break;
            }

            class_weights_in_t vec;
            while (iss >> v3_1 >> v3_2){
                vec.push_back(make_pair(v3_1, v3_2));
            }

            m[k3] = vec;
        }
    }

    cout << "finish reading data/weights.txt" << endl;

    postagger = new PerceptronTagger(weights, bias_weights, specified_tags);
    cout << "finish loading pos tagger" << endl;
}

vector<pair<string, string> > Postagger_Model::tag_sentence(string str){
    vector<string> sentence;
    istringstream iss(str);
    string word;

    while (iss >> word){
        sentence.push_back(word);
    }

    vector<tag_t> tagged_words = get_pos_tagger()->tag_sentence(sentence);

    return tagged_words;
}