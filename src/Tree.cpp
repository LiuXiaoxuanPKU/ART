#include "Tree.h"
#include <iostream>
using namespace std;

Tree::Tree() : root(new N256(nullptr, 0)){
};

Tree::~Tree(){
    delete root;
}

N* Tree::lookup(uint8_t* key, int key_size) const {
    N *node = nullptr;
    N *next_node = root;
    int key_level = 0;
    int node_level = 0;
    uint8_t common_prefix[maxPrefixLen];

    while(true) {
        node = next_node;
        if(prefixMatch(node, key, key_size,
            key_level, node_level,
            common_prefix)) {
                if(key_level == key_size) {
                    return N::getValueFromLeaf(N::getChild(0,node));
                }
                next_node = N::getChild(key[key_level],node);
                // prefix
                if(next_node == nullptr)
                    return nullptr;
                if(N::isLeaf(next_node)) {
                    return N::getValueFromLeaf(next_node);
                }
        } else {
            return nullptr;
        }
        key_level++;
    }
}



void Tree::insert(uint8_t *key, N *val, int insertkey_size){
    N *node = nullptr;
    N *next_node = root;
    N *parent_node = nullptr;
    uint8_t parent_key, node_key = 0;
    int key_level = 0;
    int node_level = 0;

    while (true) {
        parent_node = node;
        parent_key = node_key;
        node = next_node;
        uint8_t common_prefix[maxPrefixLen];

        // checkPrefix
        if(prefixMatch(node, key, insertkey_size,
                        key_level, node_level,
                        common_prefix)) {
            // insert key == prefix
            if(node->prefix_len == insertkey_size) {
                N::insertOrUpdateNode(node, parent_node, parent_key, 0, val);
            }
            // size of remainKey is 1
            if(key_level == insertkey_size - 1) {
                N::insertOrUpdateNode(node, parent_node, parent_key, key[key_level], N::setLeaf(val));
                return;
            }
            node_key = key[key_level];
            next_node = N::getChild(node_key, node);
            if(next_node == nullptr) {
                addLeaf(key_level, insertkey_size,
                    key, node, val, parent_node, parent_key);
                return;
            } else if(N::isLeaf(next_node)) {
                uint8_t prefix[maxPrefixLen];
                N* node_new = new N4(prefix,0);
                node_new->insert(0,next_node);
                if(key_level == insertkey_size - 2) {
                    node_new->insert(key[key_level+1], N::setLeaf(val));
                } else {
                    uint8_t leafPrefix[maxPrefixLen];
                    subKey(key_level+2, insertkey_size, leafPrefix, key);
                    N* leaf = new N4(leafPrefix,insertkey_size - key_level-2);
                    leaf->insert(0,N::setLeaf(val));
                    node_new->insert(key[key_level+1],leaf);
                }
                N::insertOrUpdateNode(node, parent_node, parent_key,
                        key[key_level], node_new);
                return;
            }
            key_level++;
        } else {
            auto node_new = spawn(common_prefix, node,
                        key, val, insertkey_size,
                        node_level, key_level, parent_node,parent_key);
            N::change(parent_node, parent_key, node_new);
            return;
        }
    }
}

void Tree::addLeaf(int insertkey_level, int insertkey_size,
                   uint8_t* key, N* node, N* val,
                   N* parent_node, uint8_t parent_key){
    if(insertkey_level == insertkey_size - 1) {
        N::insertOrUpdateNode(node, parent_node, parent_key,
                        key[insertkey_level],N::setLeaf(val));
        return;
    }
    uint8_t prefix[maxPrefixLen];
    subKey(insertkey_level+1, insertkey_size, prefix, key);
    N* leaf = new N4(prefix,insertkey_size-insertkey_level-1);
    leaf->insert(0,N::setLeaf(val));
    N::insertOrUpdateNode(node, parent_node, parent_key,
                        key[insertkey_level], leaf);
}

N* Tree::spawn(uint8_t *common_prefix, N* node,
               uint8_t *key, N *val, int insertkey_size,
               int node_level, int key_level,  N *parent_node,uint8_t parent_key){

    auto node_new = new N4(common_prefix, node_level);
    addLeaf(key_level, insertkey_size,
            key,node_new,val,
            node, parent_key);
    N* leaf_dup = node->duplicate();
    uint8_t leaf_key[maxPrefixLen];
    subKey(node_level+1, node->prefix_len, leaf_key, node->prefix);
    leaf_dup->setPrefix(leaf_key, node->prefix_len - node_level);
    leaf_dup->prefix_len = node->prefix_len - node_level;
    node_new->insert(node->prefix[node_level],leaf_dup);
    skipIfEmpty(node_new,leaf_dup,node->prefix[node_level]);
    return node_new;
}

void Tree::skipIfEmpty(N *node_new, N *leaf_dup, uint8_t key){
    // TODO: remove std
    std::tuple<uint8_t, N *> children[256];
    N::getChildren(leaf_dup, 0, 255, children);
    if(leaf_dup->count == 1 && get<0>(children[0])==0) {
        N::insertOrUpdateNode(node_new, nullptr, 0,
                        key, get<1>(children[0]));
        delete leaf_dup;
    }
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
bool Tree::prefixMatch(N *node,uint8_t *key, int key_size,
                       int &key_level, int &node_level,
                       uint8_t *common_prefix){
    uint8_t *node_prefix = node->prefix;
    // TODO: change node_level to i
    for(node_level=0; node_level<node->prefix_len; node_level++) {
        if(key_level+node_level >= key_size ||
                    key[key_level+node_level] != node_prefix[node_level]) {
            key_level += node_level;
            return false;
        }
        common_prefix[node_level] = node_prefix[node_level];
    }
    key_level += node_level;
    return true;
}

void Tree::subKey(int start, int end, uint8_t *subKey, uint8_t *org){
    for(int i=start; i<end; i++) {
        subKey[i-start] = org[i];
    }
}
