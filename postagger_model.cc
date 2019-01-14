#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include "maxent/maxent.h"
#include "postagger_model.h"

using namespace std;

ME_Sample Postagger_Model::sample(const vector<Postagger_Model::Token> & vt, int i){
    ME_Sample sample;

    sample.label = vt[i].pos;

    const string & w0 = vt[i].str;
    const string wp1 = i > 0 ? vt[i - 1].str : "BOS";
    const string wp2 = i > 1 ? vt[i - 2].str : "BOS";
    const string wm1 = i < (int)vt.size() - 1 ? vt[i + 1].str : "EOS";
    const string wm2 = i < (int)vt.size() - 2 ? vt[i + 2].str : "EOS";

    sample.add_feature("W0_"  + w0);
    sample.add_feature("W-1_" + wm1);
    sample.add_feature("W+1_" + wp1);
    sample.add_feature("W-2_" + wm2);
    sample.add_feature("W+2_" + wp2);

    sample.add_feature("W-10_"  + wm1 + "_" + w0);
    sample.add_feature("W0+1_"  + w0  + "_" + wp1);
    sample.add_feature("W-1+1_" + wm1 + "_" + wp1);

    char buf[1000];
    for (unsigned int j = 1; j <= 10; j++){
        if (w0.size() >= j){
            sprintf(buf, "SUF_%s", w0.substr(w0.size() - j).c_str());
            sample.add_feature(buf);
        }
        if (w0.size() >= j){
            sprintf(buf, "PRE_%s", w0.substr(0, j).c_str());
            sample.add_feature(buf);
        }
    }

    return sample;
}

vector<Postagger_Model::Token> Postagger_Model::read_line(const string & line){
    vector<Postagger_Model::Token> vs;
    istringstream is(line);
    string word, pos_tag, type;

    while (is >> word >> pos_tag >> type){
        vs.push_back(Postagger_Model::Token(word, pos_tag));
    }

    return vs;
}

void Postagger_Model::train(){
    ME_Model model;
    ifstream ifile(train_file.c_str());

    if (!ifile) {
        cerr << "error: cannot open " << train_file << endl; 
        exit(1); 
    }

    string line;
    int n = 0;
    while (getline(ifile, line)){
        vector<Postagger_Model::Token> vs = read_line(line);
        for (int j = 0; j < (int)vs.size(); j++){
            ME_Sample mes = sample(vs, j);
            model.add_training_sample(mes);
        }
        if (n++ > 10000) break;
    }

    model.use_l1_regularizer(1.0);
    // model.use_l2_regularizer(1.0);
    // model.use_SGD();
    model.set_heldout(100);
    model.train();

    postagger_model = model;
    postagger_model_loaded = true;

    model.save_to_file("postagger.model");
}

void Postagger_Model::test(){
    ME_Model model = get_postagger_model();
    ifstream ifile(test_file.c_str());

    if (!ifile){
        cerr << "error: cannot open " << test_file << endl; 
        exit(1);
    }

    int num_correct = 0;
    int num_tokens = 0;
    string line;
    while (getline(ifile, line)){
        vector<Postagger_Model::Token> vs = read_line(line);
        for (int j = 0; j < (int)vs.size(); j++){
            ME_Sample mes = sample(vs, j);
            model.classify(mes);
            if (mes.label == vs[j].pos) num_correct++;
            num_tokens++;
        }
    }

    cout << "accuracy = " << num_correct << " / " << num_tokens << " = ";
    cout << (double) num_correct / num_tokens << endl;
}

vector<Postagger_Model::Token> Postagger_Model::classify_line(const string & line){
    ME_Model model = get_postagger_model();

    vector<Postagger_Model::Token> vs;
    istringstream iss(line);
    string word;

    while (iss >> word){
        vs.push_back(Token(word, "n/a"));
    }

    for (int j = 0; j < (int) vs.size(); j++){
        ME_Sample mes = sample(vs, j);
        model.classify(mes);
        vs[j].pos = mes.label;
    }

    return vs;
}