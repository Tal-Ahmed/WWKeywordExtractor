#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "keyword_extr_model.h"
#include "maxent/maxent.h"
#include "util.h"

using namespace std;

ME_Sample generate_sample(vector<Keyword_Extractor_Model::ClassifiedToken> & classified_words, int i){
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

vector<Keyword_Extractor_Model::ClassifiedToken> read_line(const string & str){
    vector<Keyword_Extractor_Model::ClassifiedToken> classified_tokens;
    istringstream iss(str);
    string word, pos_tag, type;

    while (iss >> word >> pos_tag >> type){
        classified_tokens.push_back(Keyword_Extractor_Model::ClassifiedToken(word, pos_tag, type));
    }

    return classified_tokens;
}

void Keyword_Extractor_Model::train(){
    ME_Model model;
    ifstream fs(train_file);
    
    if (!fs){
        error("could not open " + train_file);
    }

    log("start reading " + train_file);
    string line;
    while (getline(fs, line)){
        vector<Keyword_Extractor_Model::ClassifiedToken> classified_tokens = read_line(line);

        for (int i = 0; i < classified_tokens.size(); i += 1){
            ME_Sample sample = generate_sample(classified_tokens, i);
            model.add_training_sample(sample);
        }
    }
    log("finished reading " + train_file);

    // model.use_l1_regularizer(1.0);
    // model.use_l2_regularizer(1.0);
    // model.use_SGD();
    // model.set_heldout(100);
    model.train();

    extractor_model = model;
    extractor_model_loaded = true;

    log("saving model");

    remove("extractor.model");
    model.save_to_file("extractor.model", 0.0);
}

void Keyword_Extractor_Model::test(){
    ME_Model extractor_model = get_extractor_model();

    /*
    log("loading postagger model");
    ME_Model postagger_model;
    postagger_model.load_from_file("postagger.model");
    */

    ifstream fs(test_file);
    
    if (!fs){
        error("could not open " + test_file);
    }

    int num_samples = 0;
    int num_correct = 0;

    log("start reading " + test_file);
    string line;
    while (getline(fs, line)){
        vector<Keyword_Extractor_Model::ClassifiedToken> testing_data = read_line(line);

        for (int i = 0; i < testing_data.size(); i += 1){
            ME_Sample sample = generate_sample(testing_data, i);
            extractor_model.classify(sample);
            if (sample.label == testing_data[i].type) num_correct += 1;
            num_samples += 1;
        }
    }
    log("finished reading " + test_file);  

    cout << "Extractor model accuracy = " << num_correct << " / " << num_samples << " = ";
    cout << (double) num_correct / num_samples << endl;
}

void Keyword_Extractor_Model::classify_line(vector<Keyword_Extractor_Model::ClassifiedToken> words){

}