#include "gtest/gtest.h"
#include "N.h"
using namespace std;

class NodeTest : public ::testing::Test {
};

TEST_F(NodeTest, N4Test){
	N4 testN4({},0);
	N4 child1({},0);
	N4 child2({},0);
	N4 child3({},0);
	N4 child4({},0);
	N4 child5({},0);
	testN4.insert(2, &child1);
	testN4.insert(1, &child2);
	testN4.insert(4, &child3);
	testN4.insert(3, &child4);
	ASSERT_TRUE(testN4.insert(5, &child5) == false);
	ASSERT_TRUE(testN4.getChild(2)==&child1);
	ASSERT_TRUE(testN4.getChild(1)==&child2);
	// check order
	ASSERT_TRUE(testN4.keys[0] == 1);
	ASSERT_TRUE(testN4.keys[1] == 2);
	ASSERT_TRUE(testN4.keys[2] == 3);
	ASSERT_TRUE(testN4.keys[3] == 4);

	ASSERT_TRUE(testN4.remove(1)==true);
	ASSERT_TRUE(testN4.remove(3)==true);
	ASSERT_TRUE(testN4.getChild(1)==nullptr);
	ASSERT_TRUE(testN4.keys[0] == 2);
	ASSERT_EQ(testN4.keys[1], 4);
	ASSERT_TRUE(testN4.getChild(4)==&child3);
}

// static void insertNode(N *node, N *parentNode, uint8_t keyParent, uint8_t key, N *val);
TEST_F(NodeTest, GrowTest){
	N4* parent = new N4({},0);
	N4* curNode = new N4({},0);
	N4 child1({},0);
	N4 child2({},0);
	N4 child3({},0);
	N4 child4({},0);
	N4 child5({},0);
	parent->insert(1, curNode);
	N::insertNode(curNode, parent, 1, 1, &child1);
	N::insertNode(curNode, parent, 1, 2, &child2);
	N::insertNode(curNode, parent, 1, 3, &child3);
	N::insertNode(curNode, parent, 1, 4, &child4);
	N::insertNode(curNode, parent, 1, 5, &child5);
	ASSERT_EQ(parent->getChild(1)->type,  NTypes::N16);
}

TEST_F(NodeTest, ShrinkTest){
	
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
