#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <cstdlib> // random number generation
#include "gtest/gtest.h"
#include "N.h"
#include "Tree.h"
using namespace std;

static const string file_dir = "/Users/mac/gitDevelop/ART/words.txt";
static const string out_dir = "/Users/mac/gitDevelop/ART/test/graph/";
static const int kTestSize = 234369;
static vector<uint8_t*> words;
static vector<string> words_str;
static vector<int>key_sizes;

class WordTest : public ::testing::Test {
protected:
    void SetUp() override {
        // load wordList
        ifstream infile(file_dir);
        int count = 0;
        while (infile.good() && count < kTestSize) {
            string tmp;
            infile >> tmp;
            uint8_t* key = new uint8_t[50];
            for(int i=0;i<tmp.length();i++){
                key[i] = static_cast<uint8_t>(tmp[i]);
            }
            key_sizes.push_back(int(tmp.length()));
            words.push_back(key);
            // copy to the end of vec
            words_str.push_back(tmp);
            count++;
        }
    }

    void TearDown() override {
        for(int i=0;i<words.size();i++){
            delete words[i];
        }
    }
};

TEST_F(WordTest, allWordsTest){
    Tree* test = new Tree();
    for(int i=0;i<words.size();i++){
        test->insert(words[i], reinterpret_cast<N*>(i),key_sizes[i]);
    }
    for(int i=0;i<words.size();i++){
        ASSERT_EQ(test->lookup(words[i],key_sizes[i]), reinterpret_cast<N*>(i));
    }
    delete test;
}

TEST_F(WordTest, rangeLookup){
    map<string,int> keysize_pairs;
    for(int i=0;i<words.size();i++){
        keysize_pairs.insert(pair<string,int>(words_str[i],i));
    }
    auto pair_it = keysize_pairs.begin();
    Tree* test = new Tree();
    for(int i=0;i<words.size();i++){
        test->insert(words[i], reinterpret_cast<N*>(i),key_sizes[i]);
    }
    for(int i=0;i<kTestSize;i++){
        if(i%500==0)
            cout << i << " range lookup test" << endl;
        int start_idx = rand()%(words.size());
        int end_idx = min(int(rand()%maxResultLen+start_idx),int(words.size())-1);
        // cout << "Start words: " << next(pair_it,start_idx)->first << endl;
        // cout << "End words: " << next(pair_it,end_idx)->first << endl;
        int org_start_idx = next(pair_it,start_idx)->second;
        int org_end_idx = next(pair_it, end_idx)->second;
        uint8_t* start_key = words[org_start_idx];
        uint8_t* end_key = words[org_end_idx];
        int start_size = key_sizes[org_start_idx];
        int end_size = key_sizes[org_end_idx];
        // cout << "Start size:" << start_size << endl;
        // cout << "End size" << end_size << endl;
        N* result[maxResultLen];
        int result_cnt = 0;
        test->rangeLookup(start_key, end_key,
                          start_size, end_size,
                          result, result_cnt);
        ASSERT_EQ(result_cnt,end_idx-start_idx+1);
    }
    delete test;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
