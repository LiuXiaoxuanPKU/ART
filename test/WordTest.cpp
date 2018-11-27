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

class WordTest : public ::testing::Test {
protected:
    vector<uint8_t*> words;
    vector<string> words_str;
    vector<int>key_sizes;
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

    void visTree(Tree* tree, string name){
        // BFS
        ofstream out;
        int cnt = 0;
        out.open(out_dir+name);
        out << "digraph G {"<<endl;
        N* root = tree->root;
        queue<N*>l;
        map<N*,int>visit;
        l.push(root);
        visit.insert(pair<N*,int>(root,cnt));
        while(l.size()>0) {
            N* curNode = l.front();
            l.pop();
            uint8_t children_key[256];
            N* children_p[256];
            //cout<<"("<<cnt<<")"<<endl;
            int child_cnt = 0;
            N::getChildren(curNode, 0, 255, children_key, children_p,child_cnt);
            for(unsigned int i=0; i < curNode->count; i++) {
                N* curChild = children_p[i];
                int start_id = -1;
                start_id = visit.find(curNode)->second;
                int end_id = -1;
                uint8_t curKey = children_key[i];
                if(N::isLeaf(curChild)) {
                    out << "\t";
                    int end_val = reinterpret_cast<uint64_t>(N::getValueFromLeaf(curChild));
                    if(curKey>0)
                        out << start_id <<" -> "<< end_id<<end_val<<"[label=\""<<curKey<<"\"]";
                    else
                        out << start_id <<" -> "<< end_id<<end_val<<"[label=\"\"]";
                    out <<";"<<endl;
                    continue;
                }
                if(visit.find(curChild)==visit.end()) {
                    cnt += 1;
                visit.insert(pair<N*,int>(curChild,cnt));
                l.push(curChild);
                }

                end_id = visit.find(curChild)->second;

                out << "\t";
                out << start_id <<" -> "<< end_id <<"[label=\"";
                out << curKey;
                out << "\"];"<<endl;
            }
        }
        for(auto it=visit.begin(); it!=visit.end(); ++it) {
            out << "\t"<<it->second<<"[label=\"";
            for(int j=0; j<it->first->prefix_len; j++) {
                out << it->first->prefix[j];
            }
            out << "\",shape=box];"<<endl;
        }
        out << "}";
        out.close();
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

TEST_F(WordTest, rangeLookupSingle){
    Tree* test = new Tree();
    for(int i=0;i<words.size();i++){
        test->insert(words[i], reinterpret_cast<N*>(i),key_sizes[i]);
    }
    visTree(test, "big.gv");
    // Start words: acalephae
    // End words: acquaintance
    uint8_t start_key[] = {'a','c','a','l','e','p','h','a','e'};
    uint8_t end_key[] = {'a','c','q','u','a','i','n','t','a','n','c','e'};
    N* result[maxResultLen];
    int result_cnt = 0;
    test->rangeLookup(start_key, end_key,
                      9, 12,
                      result, result_cnt);
    uint64_t end_value_idx = reinterpret_cast<uint64_t>(result[result_cnt-1]);
    cout << end_value_idx <<"()"<<endl;
    cout << words_str[end_value_idx] <<endl;
    uint64_t start_value_idx = reinterpret_cast<uint64_t>(result[0]);
    cout << words_str[start_value_idx] <<endl;
    cout << words_str[reinterpret_cast<uint64_t>(result[1])]<<endl;
    cout << words_str[reinterpret_cast<uint64_t>(result[2])]<<endl;
    delete test;
}

TEST_F(WordTest, rangeLookup){
    map<string,int> keysize_pairs;
    for(int i=0;i<words.size();i++){
        keysize_pairs.insert(pair<string,int>(words_str[i],i));
    }
    auto pair_it = keysize_pairs.begin();
    Tree* test = new Tree();
    cout << "2:"<<words.size()<<endl;
    for(int i=0;i<words.size();i++){
        test->insert(words[i], reinterpret_cast<N*>(i),key_sizes[i]);
    }
    for(int i=0;i<kTestSize;i++){
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
        // cout << "End size:" << end_size << endl;
        N* result[maxResultLen];
        int result_cnt = 0;
        test->rangeLookup(start_key, end_key,
                          start_size, end_size,
                          result, result_cnt);
        ASSERT_EQ(result_cnt,end_idx-start_idx+1);
        if(i%500==0){
            cout << i << " range lookup test" << endl;
            cout << result_cnt << endl;
        }
        // ASSERT_EQ();
        // ASSERT_EQ();
    }
    delete test;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
