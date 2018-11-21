#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include "gtest/gtest.h"
#include "N.h"
#include "Tree.h"
using namespace std;

static const string file_dir = "/Users/mac/gitDevelop/ART/words.txt";
static const string out_dir = "/Users/mac/gitDevelop/ART/test/graph/";
static const int kTestSize = 234369;
static vector<uint8_t*> words;
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

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
