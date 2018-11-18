#include "Tree.h"
#include<iostream>
using namespace std;

Tree::Tree() : root(new N256(nullptr, 0)){
};

Tree::~Tree(){
  delete root;
}

// void* Tree::lookup() const {
//
// }
//
//

void Tree::insert(uint8_t *key, N *val, int insertKeySize){
 N *node = nullptr;
 N *nextNode = root;
 N *parentNode = nullptr;
 uint8_t parentKey, nodeKey = 0;
 int keyLevel = 0;
 int nodeLevel = 0;

 while (true) {
     parentNode = node;
     parentKey = nodeKey;
     node = nextNode;
     uint8_t commonPrefix[maxPrefixLen];

     // checkPrefix
     if(prefixMatch(node, key, insertKeySize,
                    keyLevel, nodeLevel,
                    commonPrefix)) {
        if(node->prefix_len == insertKeySize){
            node->insert(0,N::setLeaf(val));
            return;
        }
        cout << "Prefix match:"<<keyLevel <<endl;
        // size of remainKey is 1
        if(keyLevel == insertKeySize - 1){
            node->insert(key[keyLevel],N::setLeaf(val));
            return;
        }
        nextNode = N::getChild(key[keyLevel], node);
        if(nextNode == nullptr){
            uint8_t leafKey[maxPrefixLen];
            subKey(keyLevel+1, insertKeySize, leafKey, key);
            auto leaf = new N4(leafKey, insertKeySize - keyLevel - 1);
            leaf->insert(0,N::setLeaf(val));
            node->insert(nodeKey, leaf);
            return;
        } else if(N::isLeaf(nextNode)){
            uint8_t prefix[maxPrefixLen];
            N* newNode = new N4(prefix,0);
            newNode->insert(key[keyLevel],N::getValueFromLeaf(nextNode));
            if(keyLevel == insertKeySize - 1){
                newNode->insert(key[keyLevel], N::setLeaf(val));
            } else {
                uint8_t splitPrefix[maxPrefixLen];
                N* leaf = new N4(splitPrefix,0);
                uint8_t leafPrefix[maxPrefixLen];
                subKey(keyLevel+1, insertKeySize, leafPrefix, key);
                leaf->setPrefix(leafPrefix, insertKeySize - keyLevel-2);
                leaf->insert(0,N::setLeaf(val));
                cout << "-----"<<unsigned(key[keyLevel+1])<<endl;
                newNode->insert(key[keyLevel+1],leaf);
            }
            node->insert(key[keyLevel],newNode);
            return;
        }
        keyLevel++;
     } else {
        // create new node to hold split results
        // nodeLevel = size(commonPrefix)
        auto newNode = spawn(commonPrefix, node,
                                key, val, insertKeySize,
                                nodeLevel, keyLevel);
        // update parent node to point to new node
        N::change(parentNode, parentKey, newNode);
     }
 }
}

N* Tree::spawn(uint8_t *commonPrefix, N* node,
               uint8_t *key, N *val, int insertKeySize,
                int nodeLevel, int keyLevel){
    auto newNode = new N4(commonPrefix, nodeLevel);
    if(keyLevel == insertKeySize - 1)
        // update
        newNode->insert(key[keyLevel],N::setLeaf(val));
    else{
        uint8_t leafKey[maxPrefixLen];
        subKey(keyLevel+1, insertKeySize, leafKey, key);
        auto leaf = new N4(leafKey,insertKeySize - keyLevel - 1);
        leaf->insert(0,N::setLeaf(val));
        newNode->insert(key[keyLevel],leaf);
    }
    N* dupLeaf = node->duplicate();
    uint8_t leafKey[maxPrefixLen];
    subKey(nodeLevel+1, node->prefix_len, leafKey, node->prefix);
    dupLeaf->setPrefix(leafKey, node->prefix_len - nodeLevel);
    newNode->insert(node->prefix[nodeLevel],dupLeaf);
    return newNode;
}

// void Tree::remove(uint8_t[] key, void *value){
//  N *node = nullptr;
//  N *nextNode = root;
//  N *parentNode = nullptr;
//  uint8_t parentKey, nodeKey = 0;
//  int level = 0;
//
//  while(true) {
//      parentNode = node;
//      parentKey = nodeKey;
//      node = nextNode;
//
//      if(prefixMatch(node, key, nextLevel, commonPrefix, remainPrefix)) {
//          nodeKey = key[level];
//          nextNode = N::getChild(nodeKey, node);
//          if (nextNode == nullptr) {
//              return;
//          }
//          if (N::isLeaf(nextNode)) {
//              // the value of leaf != val
//              if()
//                  return;
//              // the value of leaf == val
//              if(node->count == 1 && node != root) {
//
//              } else {
//                  N::removeNode(node, key[level], parentNode, parentKey);
//              }
//          }
//
//      } else {
//          // no match, just return
//          return;
//      }
//      level++;
//  }
// }
//
//
bool Tree::prefixMatch(N *node,uint8_t *key, int sizeKey,
                       int &keyLevel, int &nodeLevel,
                       uint8_t *commonPrefix){
    uint8_t *nodePrefix = node->prefix;
    for(; nodeLevel<node->prefix_len; nodeLevel++) {
        if(keyLevel+nodeLevel >= sizeKey ||
            key[keyLevel+nodeLevel] != nodePrefix[nodeLevel]) {
            keyLevel += nodeLevel;
            return false;
        }
        commonPrefix[nodeLevel] = nodePrefix[nodeLevel];
    }
    keyLevel += nodeLevel;
    return true;
}

void Tree::subKey(int start, int end, uint8_t *subKey, uint8_t *org){
    for(int i=start;i<end;i++){
        subKey[i-start] = org[start];
    }
}
