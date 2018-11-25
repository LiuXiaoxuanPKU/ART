#include "gtest/gtest.h"
#include "N.h"
using namespace std;

class NodeTest : public ::testing::Test {
protected:
void SetUp() override {
    for(int i=0; i<5; i++)
        children[i] = new N4({},0);
}

void TearDown() override {
    for(int i=0; i<5; i++)
    delete children[i];
}
N* children[5];
};


TEST_F(NodeTest, N4){
    N4 testN4({},0);
    testN4.insert(2, children[0]);
    testN4.insert(1, children[1]);
    testN4.insert(4, children[2]);
    testN4.insert(3, children[3]);
    ASSERT_TRUE(testN4.insert(5, children[4]) == false);
    ASSERT_TRUE(testN4.getChild(2)==children[0]);
    ASSERT_TRUE(testN4.getChild(1)==children[1]);
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
    ASSERT_TRUE(testN4.getChild(4)==children[2]);
}

// static void insertNode(N *node, N *parentNode, uint8_t keyParent, uint8_t key, N *val);
TEST_F(NodeTest, GrowShrink){
    N4* parent = new N4({},0);
    N4* curNode = new N4({},0);
    parent->insert(1, curNode);
    N::insertOrUpdateNode(curNode, parent, 1, 1, children[0]);
    N::insertOrUpdateNode(curNode, parent, 1, 2, children[1]);
    N::insertOrUpdateNode(curNode, parent, 1, 3, children[2]);
    N::insertOrUpdateNode(curNode, parent, 1, 4, children[3]);
    N::insertOrUpdateNode(curNode, parent, 1, 5, children[4]);
    ASSERT_EQ(parent->getChild(1)->type,  NTypes::N16);
    // origin node has been removed
    curNode = static_cast<N4 *>(parent->getChild(1));
    //removeNode(N *node, N *parentNode, uint8_t keyParent, uint8_t key);
    N::removeNode(curNode, parent, 1, 1);
    ASSERT_EQ(parent->getChild(1)->type,  NTypes::N16);
    N::removeNode(curNode, parent, 1, 2);
    ASSERT_EQ(parent->getChild(1)->type,  NTypes::N4);
}

TEST_F(NodeTest, Leaf){
    for(int i = 0; i < 5; i++)
    ASSERT_TRUE(N::isLeaf(children[i]) == false);
    for(int i = 0; i < 5; i++) {
    ASSERT_TRUE(N::isLeaf(N::setLeaf(children[i])) == true);
    }
}

TEST_F(NodeTest, getChild){
    N* root = new N4({},0);
    N* curNode = new N4({},0);
    root->insert(9,curNode);
    for(uint8_t i=0; i<5; i++)
    N::insertOrUpdateNode(curNode, root, 9, i, children[i]);
    ASSERT_EQ(N::getChild(9,root)->type, NTypes::N16);
    for(uint8_t i=0; i<5; i++)
    ASSERT_TRUE(N::getChild(i,N::getChild(9,root))==children[i]);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
