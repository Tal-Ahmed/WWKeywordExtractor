#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "keyword_extr_model.h"
#include "maxent/maxent.h"

using namespace std;

typedef vector<Keyword_Extractor_Model::ClassifiedToken> classified_tokens_t;

ME_Sample Keyword_Extractor_Model::generate_sample(classified_tokens_t & classified_words, int i){
    ME_Sample sample;
    sample.label = classified_words[i].type;

    int num_samples = (int) classified_words.size();

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

vector<Keyword_Extractor_Model::ClassifiedToken> Keyword_Extractor_Model::read_line(const string & str){
    classified_tokens_t classified_tokens;
    istringstream iss(str);
    string word, pos_tag, type;

    while (iss >> word){
        while (iss >> pos_tag && pos_tag != "(" && pos_tag != "," && pos_tag != ")" && pos_tag != "." && pos_tag != ":" && pos_tag != ";" && pos_tag != "CC" && pos_tag != "CD" && pos_tag != "DT" && 
                pos_tag != "EX" && pos_tag != "FW" && pos_tag != "IN" && pos_tag != "JJ" && pos_tag != "JJR" && 
                pos_tag != "JJS" && pos_tag != "LS" && pos_tag != "MD" && pos_tag != "NN" && 
                pos_tag != "NNS" && pos_tag != "NNP" && pos_tag != "NNPS" && pos_tag != "PDT" && 
                pos_tag != "POS" && pos_tag != "PRP" && pos_tag != "PRP$" && pos_tag != "RB" && 
                pos_tag != "RBR" && pos_tag != "RBS" && pos_tag != "RP" && pos_tag != "TO" && 
                pos_tag != "UH" && pos_tag != "VB" && pos_tag != "VBD" && pos_tag != "VBG" && 
                pos_tag != "VBN" && pos_tag != "VBP" && pos_tag != "VBZ" && pos_tag != "WDT" && 
                pos_tag != "WP" && pos_tag != "WP$" && pos_tag != "WRB"){
            word = word + " " + pos_tag;
        }

        iss >> type;
        classified_tokens.push_back(Keyword_Extractor_Model::ClassifiedToken(word, pos_tag, type));
    }

    return classified_tokens;
}

void Keyword_Extractor_Model::train(){
    cout << "start training keyword extractor" << endl;

    ME_Model model;
    ifstream fs(train_file);
    
    if (!fs){
        cerr << "could not open " + train_file << endl;
        exit(1);
    }

    cout << "start reading " + train_file << endl;
    string line;
    while (getline(fs, line)){
        vector<Keyword_Extractor_Model::ClassifiedToken> classified_tokens = read_line(line);

        for (int i = 0; i < (int) classified_tokens.size(); i += 1){
            ME_Sample sample = generate_sample(classified_tokens, i);
            model.add_training_sample(sample);
        }
    }
    cout << "finished reading " + train_file << endl;

    //model.use_l1_regularizer(1.0);
    //model.use_l2_regularizer(1.0);
    //model.use_SGD();
    //model.set_heldout(100);
    model.train();

    extractor_model = model;
    extractor_model_loaded = true;

    cout << "saving model" << endl;

    remove("extractor.model");
    model.save_to_file("extractor.model", 0.0);
    cout << "finish training keyword extractor" << endl;
}

void Keyword_Extractor_Model::test(){
    cout << "start testing keyword extractor" << endl;
    ME_Model extr_model = get_extractor_model();

    ifstream fs(test_file);
    
    if (!fs){
        cerr << "could not open " + test_file << endl;
        exit(1);
    }

    int num_samples = 0;
    int num_correct = 0;
        int x = 0;
        int y = 0;
    cout << "start reading " + test_file << endl;
    string line;
    while (getline(fs, line)){
        vector<Keyword_Extractor_Model::ClassifiedToken> testing_data = read_line(line);

        ostringstream oss;
        for (int i = 0; i < testing_data.size(); i += 1){
            oss << testing_data[i].word << " ";
        }

        vector<pair<string, string> > tagged_data = postagger_model.tag_sentence(oss.str());

        for (int i = 0; i < (int) testing_data.size(); i += 1){
            if (testing_data[i].pos_tag != tagged_data[i].second) x += 1;
            testing_data[i].pos_tag = tagged_data[i].second; y += 1;

            ME_Sample sample = generate_sample(testing_data, i);
            extr_model.classify(sample);
            if (sample.label == testing_data[i].type) num_correct += 1;
            num_samples += 1;
        }
    }
    cout << "finished reading " + test_file << endl;
cout << "X: " << x << " Y: " << y << endl; 
    cout << "Extractor model accuracy = " << num_correct << " / " << num_samples << " = ";
    cout << 100 * ((double) num_correct / num_samples) << "%" << endl;
    cout << "finish testing keyword extractor" << endl;
}

void Keyword_Extractor_Model::classify_line(classified_tokens_t words){

}