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
            key_level, node_level, common_prefix)) {
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

void Tree::rangeLookupHelper(N* node, int  key_level, int key_size,
                        uint8_t *start_key, uint8_t *end_key,
                        N **result,int &result_cnt,
                        bool is_start, bool is_end){
    //cout << "In range:"<<key_level<<" "<<is_start<<" "<<is_end<<" "<<node->prefix_len<<endl;
    uint8_t cur_start = 0;
    uint8_t cur_end = 255;
    int start_level = key_level;
    int end_level = key_level;
    if (is_start) {
        int i = 0;
        for(;i<node->prefix_len;i++){
            if(start_level < key_size && node->prefix[i]==start_key[start_level])
                start_level++;
            else if (start_level < key_size && node->prefix[i] < start_key[start_level])
                return;
            else {
                is_start = false;
                cur_start = 0;
                break;
            }
        }
        if(i == node->prefix_len)
            cur_start = start_key[start_level];
    }
    if (is_end) {
        int i = 0;
        for(;i<node->prefix_len;i++){
            if(end_level < key_size && node->prefix[i]==end_key[end_level])
                end_level++;
            else if (end_level < key_size &&node->prefix[i] > end_key[end_level])
                return;
            else {
                //cout << "prefix:"<<unsigned(node->prefix[i])<<" "<<unsigned(end_key[end_level])<<endl;
                is_end = false;
                cur_end = 255;
                break;
            }
        }
        //cout << i << "()"<<end_level<<" "<<unsigned(end_key[end_level])<<endl;
        if(i == node->prefix_len)
            cur_end = end_key[end_level];
    }
    key_level = (start_level>end_level)?start_level:end_level;
    if (is_start == false && is_end == false){
        key_level += node->prefix_len;
    }

    uint8_t children_key[256];
    N* children_p[256];
    int child_cnt = 0;
    cur_end = (cur_end==255)?255:cur_end+1;
    N::getChildren(node, cur_start, cur_end,
                children_key, children_p, child_cnt);
    //cout << unsigned(cur_start) << " "<< unsigned(cur_end)<<"*"<<endl;
    //cout << node->prefix_len << " " << child_cnt << endl;
    key_level++;
    for(int i=0;i<child_cnt;i++){
        if(N::isLeaf(children_p[i])){
            result[result_cnt++] = N::getValueFromLeaf(children_p[i]);
            //cout << "Get result:" << reinterpret_cast<uint64_t>(N::getValueFromLeaf(children_p[i]))<<endl;
            continue;
        }
        bool start = (is_start==true && i==0)?true:false;
        //cout << "i:"<<i<<endl;
        bool end = (is_end==true && i==child_cnt-1)?true:false;
        rangeLookupHelper(children_p[i],key_level, key_size,
                          start_key, end_key,
                          result,result_cnt,start,end);
    }
}

void Tree::rangeLookup(uint8_t *start_key, uint8_t *end_key, int key_size,
                       N **result, int &result_cnt){
    rangeLookupHelper(root, 0, key_size, start_key, end_key,result,result_cnt,true,true);
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
                    uint8_t leaf_prefix[maxPrefixLen];
                    subKey(key_level+2, insertkey_size, leaf_prefix, key);
                    N* leaf = new N4(leaf_prefix,insertkey_size - key_level-2);
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
            key, node_new, val,
            node, parent_key);
    N* leaf_dup = node->duplicate();
    uint8_t leaf_key[maxPrefixLen];
    subKey(node_level+1, node->prefix_len, leaf_key, node->prefix);
    leaf_dup->setPrefix(leaf_key, node->prefix_len - node_level - 1);
    leaf_dup->prefix_len = node->prefix_len - node_level - 1;
    node_new->insert(node->prefix[node_level],leaf_dup);
    skipIfEmpty(node_new, leaf_dup, node->prefix[node_level]);
    return node_new;
}

void Tree::skipIfEmpty(N *node_new, N *leaf_dup, uint8_t key){
    // TODO: remove std
    uint8_t children_key[256];
    N* children_p[256];
    int child_cnt = 0;
    N::getChildren(leaf_dup, 0, 255, children_key, children_p,child_cnt);
    if(leaf_dup->prefix_len == 0 && leaf_dup->count == 1 && children_key[0]==0) {
        N::insertOrUpdateNode(node_new, nullptr, 0,
                        key, children_p[0]);
        delete leaf_dup;
    }
}

bool Tree::remove(uint8_t* key, int deletekey_size){
    // N *node = nullptr;
    // N *next_node = root;
    // N *parent_node = nullptr;
    // uint8_t parent_key, node_key = 0;
    // int key_level = 0;
    // int node_level = 0;
    //
    // while(true) {
    //     parent_node = node;
    //     parent_key = node_key;
    //     node = next_node;
    //     uint8_t common_prefix[maxPrefixLen];
    //     if(prefixMatch(node, key, deletekey_size,
    //                     key_level, node_level,
    //                     common_prefix)) {
    //         if(key_level == deletekey_size)
    //             node_key = 0;
    //         else
    //             node_key = key[key_level];
    //         next_node = N::getChild(node_key, node);
    //         if (next_node == nullptr) {
    //             return false;
    //         }
    //         if (N::isLeaf(next_node) &&
    //                 (key_level == deletekey_size - 1 || key_level == deletekey_size)) {
    //             N::removeNode(node, parent_node, parent_key, node_key);
    //             if(node->count == 1){
    //                 node->getFirstChild();
    //                 child->setPrefix();
    //                 parent_node->insert(parent_key, child);
    //                 delete node;
    //             }
    //             return true;
    //         }
    //     } else {
    //         // no match, just return
    //         return false;
    //     }
    //     key_level++;
    // }
    return true;
}


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
