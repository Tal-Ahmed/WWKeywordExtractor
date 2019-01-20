#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <regex>
#include "postagger_model.h"
#include "mltk/_ctagger.cc"

using namespace std;

extern unordered_map<string, unordered_map<string, string> > keywords;

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

string make_regex(string keyword){
    ostringstream oss;
    oss << "[\\s|(|,|-|/](";
    oss << keyword;
    oss << ")[\\s|)|,|-|.|/]";
    return oss.str();
}

vector<pair<string, string> > Postagger_Model::tag_sentence(string str){
    unordered_map<int, unordered_map<int, string> > history;
    for (unordered_map<string, unordered_map<string, string> >::iterator iter = keywords.begin(); iter != keywords.end(); ++iter){
        string keyword = iter->first;
        regex e(make_regex(keyword), regex_constants::icase);
        smatch m;
        regex_search(str, m, e);

        // if match
        if (m.str(1) != ""){            
            int start_index = m.position(1);
            int end_index = m.position(1) + m.str(1).length() - 1;
            history[start_index][end_index] = keyword;

            for (int i = start_index; i <= end_index; i += 1){
                str[i] = '#';
            }
        }
    }

    istringstream iss(str);
    vector<string> sentence;
    string word;
    int start_index = 0, end_index = -1;

    while (iss >> word){
        start_index = end_index + 1;
        end_index = start_index + word.length() - 1;

        if (history.find(start_index) != history.end() && 
            history[start_index].find(end_index) != history[start_index].end()){
            word = history[start_index][end_index];
        }

        sentence.push_back(word);
    }

    vector<tag_t> tagged_words = get_pos_tagger()->tag_sentence(sentence);

    return tagged_words;
}