#include "gtest/gtest.h"
#include "N.h"
#include "Tree.h"
using namespace std;

class TreeTest : public ::testing::Test {
protected:
	void SetUp() override {
	}

	void TearDown() override {
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


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
