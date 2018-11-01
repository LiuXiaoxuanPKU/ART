//
//  N16.cpp
//  xiaoxuan
//
//  Created by MAC on 2018/10/25.
//  Copyright © 2018 xiaoxuan. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include <iostream>
#include "N.hpp"
using namespace std;

bool N16::insert(uint8_t k, N* node){
    if(count==16)
        return false;
    for(unsigned i=0;i<16;i++){
        if(children[i] == nullptr){
            keys[i] = k;
            children[i] = node;
            count++;
            return true;
        }
    }
    cout << " Wrong count in inserting N4" << endl;
    assert(false);
}

N* N4::getChild(uint8_t k){
    for(int i=0;i<16;i++){
        if(children[i]!=nullptr && this->keys[i]==k)
            return children[i];
    }
    return nullptr;
}

bool N16::remove(uint8_t k){
    for(int i=0;i<16;i++){
        if(children[i]!=nullptr && keys[i]==k){
            count--;
            children[i] = nullptr;
            return true;
        }
    }
    cout << " Remove N4 element not exist" << endl;
    assert(false);
}

void N16::change(uint8_t key, N *val){
    for(int i=0;i<16;i++){
        if(children[i] != nullptr && keys[i] == key){
            children[i] = val;
            return;
        }
    }
}
