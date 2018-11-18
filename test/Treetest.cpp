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
			int childCount = 0;
			N::getChilren(curNode, 0, 255, children, childCount);
			for(int i=0;i < childCount;i++){
				N* curChild = get<1>(children[i]);
				int start_id = visit.find(curNode)->second;
				int end_id = -1;
				if(N::isLeaf(curChild)){
					out << "\t";
					end_id = reinterpret_cast<uint64_t>(N::getValueFromLeaf(curChild));
					out << start_id <<" -> "<< end_id<<endl;
					continue;
				}
				if(visit.find(curChild)==visit.end()){
					cnt += 1;
					visit.insert(pair<N*,int>(curChild,cnt));
				}

				end_id = visit.find(curChild)->second;

				out << "\t";
				out << start_id <<" -> "<< end_id <<"(";
				for(int j=0;j<curChild->prefix_len;j++)
					out << curChild->prefix[j];
				out << ");"<<endl;

			}
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
	uint64_t val1 = 123;
	uint64_t val2 = 456;
	test->insert(key1, reinterpret_cast<N*>(val1), 1);
	test->insert(key2, reinterpret_cast<N*>(val2),3);
	visTree(test, "Test");
	delete test;
}



int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
