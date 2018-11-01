//
//  N.hpp
//  xiaoxuan
//
//  Created by MAC on 2018/10/25.
//  Copyright © 2018 xiaoxuan. All rights reserved.
//

#ifndef N_hpp
#define N_hpp

#include <stdio.h>
#include <stdint.h>    // integer types

static const unsigned maxPrefixLen = 9;
enum class NTypes: uint8_t{
    N4 = 0,
    N16 = 1,
    N48 = 2,
    N256 = 3
};

class N{
public:
    // node type
    NTypes type;
    
    // number of childrens
    uint8_t count = 0;
    
    // length of compressed path
    uint32_t prefix_len;
    
    // compressed path
    uint8_t prefix[maxPrefixLen];
    
    N(NTypes _type, const uint8_t* _prefix, uint32_t _prefix_len)
    :type(_type),prefix_len(_prefix_len){
        for(int i=0;i<_prefix_len;i++)
            prefix[i] = _prefix[i];
    };
    
    template<typename curN, typename biggerN>
    static void insertGrow(curN* n, uint8_t k, N* node, uint8_t key_par, N* parent);
    
    template<typename curN, typename smallerN>
    static void removeAndShrink(curN* n, uint8_t k);
    
    
    
    static void change(N *node, uint8_t key, N *val);
    
};

class N4:public N{
public:
    uint8_t keys[4];
    N* children[4] = {nullptr, nullptr, nullptr, nullptr};
    N* getChild(const uint8_t k)const;
    
    N4(const uint8_t* prefix, uint32_t prefix_len):N(NTypes::N4, prefix, prefix_len){};
    bool insert(uint8_t key, N*node);
    bool remove(uint8_t key);
    void change(uint8_t key, N*val);
    N* getChild(uint8_t key);
};

class N16:public N{
public:
    uint8_t keys[16];
    N* children[16];
    
    N16(const uint8_t* prefix, uint32_t prefix_len):N(NTypes::N4, prefix, prefix_len){};
    // TODO
    bool insert(uint8_t key, N*node);
    bool remove(uint8_t key);
    void change(uint8_t key, N*val);
    N* getChild(uint8_t key);

};

class N48:public N{
public:
    uint8_t child_index[256];
    N* children[48];
    static const uint8_t empty_marker = 48;
    
    N48(const uint8_t* prefix, uint32_t prefix_len):N(NTypes::N4, prefix, prefix_len){};
    bool insert(uint8_t key, N*n);
    bool remove(uint8_t key);
    void change(uint8_t key, N*val);
    N* getChild(uint8_t k) const;
    
};

class N256:public N{
public:
    N* children[256];
    
    N256(const uint8_t* prefix, uint32_t prefix_len):N(NTypes::N4, prefix, prefix_len){};
    bool insert(uint8_t key, N*n);
    bool remove(uint8_t key);
    void change(uint8_t key, N*val);
    N* getChild(uint8_t k) const;
};


    
#endif /* N_hpp */
