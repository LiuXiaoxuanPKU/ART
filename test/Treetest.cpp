#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <tuple>
#include "gtest/gtest.h"
#include "N.h"
#include "Tree.h"
using namespace std;

string base_dir = "/Users/mac/gitDevelop/ART/test/graph/";
class TreeTest : public ::testing::Test {
protected:
	void SetUp() override {
	}

	void TearDown() override {
	}

	void visTree(Tree* tree, string name){
		// BFS
		ofstream out;
		int cnt = 0;
		out.open(base_dir+name);
		out << "digraph G {"<<endl;
		N* root = tree->root;
		queue<N*>l;
		map<N*,int>visit;
		l.push(root);
		visit.insert(pair<N*,int>(root,cnt));
		while(l.size()>0){
			N* curNode = l.front();
			l.pop();
			std::tuple<uint8_t, N *> children[256];
			cout<<"("<<cnt<<")"<<endl;
			N::getChildren(curNode, 0, 255, children);
			for(unsigned int i=0;i < curNode->count;i++){
				N* curChild = get<1>(children[i]);
				int start_id = -1;
				start_id = visit.find(curNode)->second;
				int end_id = -1;
				uint8_t curKey = get<0>(children[i]);
				if(N::isLeaf(curChild)){
					out << "\t";
					int end_val = reinterpret_cast<uint64_t>(N::getValueFromLeaf(curChild));
					out << start_id <<" -> "<< end_id<<end_val<<"[label=\""<<unsigned(curKey)<<"\"]";
					out <<";"<<endl;
					continue;
				}
				if(visit.find(curChild)==visit.end()){
					cnt += 1;
					visit.insert(pair<N*,int>(curChild,cnt));
					l.push(curChild);
				}

				end_id = visit.find(curChild)->second;

				out << "\t";
				out << start_id <<" -> "<< end_id <<"[label=\"";
				out << unsigned(curKey);
				out << "\"];"<<endl;
			}

		}
		for(auto it=visit.begin();it!=visit.end();++it){
			out << "\t"<<it->second<<"[label=\"";
			for(int j=0;j<it->first->prefix_len;j++){
				out << unsigned(it->first->prefix[j]);
			}
			out << "\",shape=box];"<<endl;
		}
		out << "}";
		out.close();
	}
};

TEST_F(TreeTest, checkPrefixOne){
	uint8_t prefix[] = {1,2,3,4,5};
	uint8_t key1[] = {1};
	uint8_t key5[] = {1,2,3,4,5};
	uint8_t key6[] = {1,2,3,4,5,6};
	uint8_t key7[] = {1,2,4,5};
	uint8_t commonPrefix[5];
	int keyLevel = 0;
	int nodeLevel = 0;
	int sizeKey = 1;
	auto testN = new N4(prefix, 5);
	// ************************TEST for key1***********************************
	ASSERT_FALSE(Tree::prefixMatch(testN, key1, sizeKey,
								  keyLevel, nodeLevel, commonPrefix));
	ASSERT_EQ(keyLevel, 1);
	ASSERT_EQ(nodeLevel, 1);
	ASSERT_EQ(commonPrefix[0], 1);
    // ************************TEST for key5***********************************
	memset(commonPrefix, 0, sizeof(commonPrefix));
	sizeKey = 5;
	keyLevel = nodeLevel = 0;
	ASSERT_TRUE(Tree::prefixMatch(testN, key5, sizeKey,
		keyLevel, nodeLevel, commonPrefix));
	ASSERT_EQ(keyLevel, 5);
	ASSERT_EQ(nodeLevel, 5);
	ASSERT_EQ(commonPrefix[0], unsigned(1));
	// ************************TEST for key6***********************************
	memset(commonPrefix, 0, sizeof(commonPrefix));
	sizeKey = 6;
	keyLevel = nodeLevel = 0;
	ASSERT_TRUE(Tree::prefixMatch(testN, key6, sizeKey,
								  keyLevel, nodeLevel, commonPrefix));
	ASSERT_EQ(commonPrefix[4], unsigned(5));
	// ************************TEST for key7***********************************
	memset(commonPrefix, 0, sizeof(commonPrefix));
	sizeKey = 4;
	keyLevel = nodeLevel = 0;
	ASSERT_FALSE(Tree::prefixMatch(testN, key7, sizeKey,
								  keyLevel, nodeLevel, commonPrefix));
	ASSERT_EQ(keyLevel, 2);
	ASSERT_EQ(nodeLevel, 2);
	ASSERT_EQ(commonPrefix[1], unsigned(2));
	delete testN;
}

TEST_F(TreeTest, BaseInsert){
	Tree *test = new  Tree();
	uint8_t key1[] = {1};
	uint8_t key2[] = {1,4,5};
	uint8_t key3[] = {2};
	uint8_t key4[] = {2,5};
	uint8_t key5[] = {1,4,5,7,8,9};
	uint8_t key6[] = {1,4,5,7,8,7};
	uint64_t val1 = 123;
	uint64_t val2 = 145;
	uint64_t val3 = 2;
	uint64_t val4 = 25;
	uint64_t val5 = 145789;
	uint64_t val6 = 145787;
	test->insert(key1, reinterpret_cast<N*>(val1), 1);
	test->insert(key2, reinterpret_cast<N*>(val2),3);
	test->insert(key3, reinterpret_cast<N*>(val3),1);
	test->insert(key4, reinterpret_cast<N*>(val4),2);
	test->insert(key5, reinterpret_cast<N*>(val5),6);
	// test->insert(key6, reinterpret_cast<N*>(val6),6);
	visTree(test, "graph1.gv");
	delete test;
}



int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
