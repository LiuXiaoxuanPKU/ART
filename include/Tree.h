#ifndef TREE_H
#define TREE_H

#include "N.h"

static const unsigned maxResultLen = 1000;

class Tree{
public:
    N* root;
    Tree();
    N* lookup(uint8_t *key,int sizeKey)const;
    void rangeLookup(uint8_t *start_key, uint8_t *end_key,
                    int start_size, int end_size,
                    N **result, int &result_cnt);
    void rangeLookupHelper(N* node, int  key_level, int key_size,
                            uint8_t *start_key, uint8_t *end_key,
                            N **result,int &result_cnt,
                            bool is_start, bool is_end);
    void insert(uint8_t *key, N* val, int keySize);
    bool remove(uint8_t *key, int deletekey_size);
    ~Tree();

    static void subKey(int start, int end, uint8_t *subKey, uint8_t *org);
    static bool prefixMatch(N *node,uint8_t *key, int sizeKey,
                   int &keyLevel, int &nodeLevel,
                   uint8_t *commonPrefix);
    static int getSize(uint8_t *key);
    static N* spawn(uint8_t *commonPrefix, N* node,
                  uint8_t *key, N *value, int insertKeySize,
                  int nodeLevel, int keyLevel,N* parentNode, uint8_t parentKey);
    static void addLeaf(int insertKeyLevel, int insertKeySize,
                    uint8_t* key, N* node, N* val, N* parentNode, uint8_t parentKey);
    static void skipIfEmpty(N *newNode, N *dupLeaf, uint8_t key);

};

#endif
