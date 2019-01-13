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

void read_data(string file_name, vector<ClassifiedToken> &classified_tokens){
    ifstream *fs;
    
    try {
        fs = new ifstream(file_name.c_str());
        fs->exceptions(ios_base::failbit);
    } catch (ios_base::failure){
        error("could not open " + file_name);
    }

    log("start reading " + file_name);
    string line;
    while (getline(*fs, line)){
        istringstream iss(line);

        // train.txt/test.txt row format: <word> <pos_tag> <keyword/not>
        string word, pos_tag, type;
        iss >> word >> pos_tag >> type;

        ClassifiedToken cw(word, pos_tag, type);
        classified_tokens.push_back(cw);
    }
    log("finished reading " + file_name);

    delete fs;
}

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

void train(){
    ME_Model model;

    vector<ClassifiedToken> training_data;
    read_data("data/train.txt", training_data);

    int num_samples = training_data.size();

    for (int i = 0; i < num_samples; i += 1){
        ME_Sample sample = generate_sample(training_data, i);
        model.add_training_sample(sample);
    }

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
    vector<ClassifiedToken> testing_data;
    read_data("data/test.txt", testing_data);

    int num_samples = testing_data.size();
    int num_correct = 0;

    for (int i = 0; i < num_samples; i += 1){
        ME_Sample sample = generate_sample(testing_data, i);
        extractor_model.classify(sample);
        if (sample.label == testing_data[i].type) num_correct += 1;
    }

    cout << "accuracy = " << num_correct << " / " << num_samples << " = ";
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