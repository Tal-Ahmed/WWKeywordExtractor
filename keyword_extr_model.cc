#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "maxent/maxent.h"
#include "util.h"

using namespace std;

struct ClassifiedToken {
    string word, pos_tag, type;
    ClassifiedToken(const string & w, const string & pos, const string & t):
        word(w), pos_tag(pos), type(t) {}
};

ME_Sample generate_sample(vector<ClassifiedToken> & classified_words, int i){
    ME_Sample sample;
    sample.label = classified_words[i].type;

    int num_samples = classified_words.size();

    const string word = classified_words[i].word;
    const string pos = classified_words[i].pos_tag;
    const string prev_pos = i - 1 >= 0 ? classified_words[i - 1].pos_tag: "BOS";
    const string prev_prev_pos = i - 2 >= 0 ? classified_words[i - 2].pos_tag: "BOS";
    const string next_pos = i + 1 < num_samples ? classified_words[i + 1].pos_tag: "EOS";
    const string next_next_pos = i + 2 < num_samples ? classified_words[i + 2].pos_tag: "EOS";

    sample.add_feature("word_" + word);
    sample.add_feature("pos_" + pos);
    sample.add_feature("prev_pos_" + prev_pos);
    sample.add_feature("prev_prev_pos_" + prev_prev_pos);
    sample.add_feature("next_pos_" + next_pos);
    sample.add_feature("next_next_pos_" + next_next_pos);

    return sample;
}

vector<ClassifiedToken> read_line(const string & str){
    vector<ClassifiedToken> classified_tokens;
    istringstream iss(str);
    string word, pos_tag, type;

    while (iss >> word >> pos_tag >> type){
        classified_tokens.push_back(ClassifiedToken(word, pos_tag, type));
    }

    return classified_tokens;
}

void train(){
    ME_Model model;
    ifstream fs("data/train.txt");
    
    if (!fs){
        error("could not open data/train.txt");
    }

    log("start reading data/train.txt");
    string line;
    while (getline(fs, line)){
        vector<ClassifiedToken> classified_tokens = read_line(line);

        for (int i = 0; i < classified_tokens.size(); i += 1){
            ME_Sample sample = generate_sample(classified_tokens, i);
            model.add_training_sample(sample);
        }
    }
    log("finished reading data/train.txt");

    // model.use_l1_regularizer(1.0);
    // model.use_l2_regularizer(1.0);
    // model.use_SGD();
    // model.set_heldout(100);
    model.train();

    log("saving model");

    remove("extractor.model");
    model.save_to_file("extractor.model", 0.0);
}

void test(){
    log("loading extractor model");
    ME_Model extractor_model;
    extractor_model.load_from_file("extractor.model");

    /*
    log("loading postagger model");
    ME_Model postagger_model;
    postagger_model.load_from_file("postagger.model");
    */

    ifstream fs("data/test.txt");
    
    if (!fs){
        error("could not open data/test.txt");
    }

    int num_samples = 0;
    int num_correct = 0;

    log("start reading data/test.txt");
    string line;
    while (getline(fs, line)){
        vector<ClassifiedToken> testing_data = read_line(line);

        for (int i = 0; i < testing_data.size(); i += 1){
            ME_Sample sample = generate_sample(testing_data, i);
            extractor_model.classify(sample);
            if (sample.label == testing_data[i].type) num_correct += 1;
            num_samples += 1;
        }
    }
    log("finished reading data/test.txt");  

    cout << "Extractor model accuracy = " << num_correct << " / " << num_samples << " = ";
    cout << (double) num_correct / num_samples << endl;
}

int main(int argc, char *argv[]){
    if (argc != 2){
        cerr << "Usage: " << argv[0] << " [train/test] " << endl;
        return 1;
    }

    string cmd(argv[1]);
    if (cmd == "train"){
        train();
    } else if (cmd == "test"){
        test();
    }

    return 0;
}