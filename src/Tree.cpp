#include "Tree.h"
#include <iostream>
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
            // insert key == prefix
			if(node->prefix_len == insertKeySize) {
                N::insertOrUpdateNode(node, parentNode, parentKey, 0, val);
			}
			// size of remainKey is 1
			if(keyLevel == insertKeySize - 1) {
                N::insertOrUpdateNode(node, parentNode, parentKey, key[keyLevel], N::setLeaf(val));
                return;
			}
			nextNode = N::getChild(key[keyLevel], node);
			nodeKey = key[keyLevel];
			if(nextNode == nullptr) {
				addLeaf(keyLevel, insertKeySize,
				        key, node, val, parentNode, parentKey);
				return;
			} else if(N::isLeaf(nextNode)) {
				uint8_t prefix[maxPrefixLen];
				N* newNode = new N4(prefix,0);
				newNode->insert(0,nextNode);
				if(keyLevel == insertKeySize - 2) {
					newNode->insert(key[keyLevel+1], N::setLeaf(val));
				} else {
					uint8_t leafPrefix[maxPrefixLen];
					subKey(keyLevel+2, insertKeySize, leafPrefix, key);
                    N* leaf = new N4(leafPrefix,insertKeySize - keyLevel-2);
					leaf->insert(0,N::setLeaf(val));
					newNode->insert(key[keyLevel+1],leaf);
				}
                std::tuple<uint8_t, N *> children[256];
                cout << unsigned(key[keyLevel])<<endl;
                N::insertOrUpdateNode(node, parentNode, parentKey,
                                        key[keyLevel], newNode);
                N::getChildren(node, 0, 255, children);
				return;
			}
			keyLevel++;
		} else {
			// cout << "Prefix do not match "<<insertKeySize<<endl;
			// create new node to hold split results
			// nodeLevel = size(commonPrefix)
			auto newNode = spawn(commonPrefix, node,
			                     key, val, insertKeySize,
			                     nodeLevel, keyLevel);
			// cout << "Parent Key:"<<unsigned(parentKey)<<endl;
			// update parent node to point to new node
			N::change(parentNode, parentKey, newNode);
			return;
		}
	}
}
void Tree::addLeaf(int insertKeyLevel, int insertKeySize,
                   uint8_t* key, N* node, N* val,
                   N* parentNode, uint8_t parentKey){
	uint8_t prefix[maxPrefixLen];
	subKey(insertKeyLevel+1, insertKeySize, prefix, key);
	N* leaf = new N4(prefix,insertKeySize-insertKeyLevel-1);
	leaf->insert(0,N::setLeaf(val));
    N::insertOrUpdateNode(node, parentNode, parentKey,
                            key[insertKeyLevel], leaf);
}

N* Tree::spawn(uint8_t *commonPrefix, N* node,
               uint8_t *key, N *val, int insertKeySize,
               int nodeLevel, int keyLevel){
	// for(int i=0; i<nodeLevel; i++)
	// 	cout << unsigned(commonPrefix[i])<<"*";
	// auto newNode = new N4(commonPrefix, nodeLevel);
	// addLeaf(keyLevel, insertKeySize,key,newNode,val);
	// N* dupLeaf = node->duplicate();
	// uint8_t leafKey[maxPrefixLen];
	// subKey(nodeLevel+1, node->prefix_len, leafKey, node->prefix);
	// dupLeaf->setPrefix(leafKey, node->prefix_len - nodeLevel);
    // dupLeaf->prefix_len = node->prefix_len - nodeLevel;
	// newNode->insert(node->prefix[nodeLevel],dupLeaf);
    // skipIfEmpty(newNode,dupLeaf,node->prefix[nodeLevel]);
	// return newNode;
}

void Tree::skipIfEmpty(N *newNode, N *dupLeaf, uint8_t key){
    // std::tuple<uint8_t, N *> children[256];
    // N::getChildren(dupLeaf, 0, 255, children);
    // if(dupLeaf->count == 1 && get<0>(children[0])==0){
    //     cout <<"In delete"<<unsigned(key)<<endl;
    //     newNode->insert(key, get<1>(children[0]));
    //     cout << get<1>(children[0])<<"*"<<endl;
    //     cout << N::getChild(key,newNode)<<endl;
    //     delete dupLeaf;
    // }
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
	for(nodeLevel=0; nodeLevel<node->prefix_len; nodeLevel++) {
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
	for(int i=start; i<end; i++) {
		subKey[i-start] = org[i];
	}
}
