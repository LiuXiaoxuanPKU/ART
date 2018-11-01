//
//  N48.cpp
//  xiaoxuan
//
//  Created by MAC on 2018/10/25.
//  Copyright © 2018 xiaoxuan. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include "N.hpp"

bool N48::insert(uint8_t k, N* n){
    if(count==48)
        return false;
    unsigned pos = 0;
    for(;children[pos]!=nullptr;pos++);
    children[pos] = n;
    child_index[k] = pos;
    count++;
    return true;
}

bool N48::remove(uint8_t k){
    assert(child_index[k] != empty_marker);
    children[child_index[k]] = nullptr;
    child_index[k] = empty_marker;
    count --;
    assert(getChild(k) == nullptr);
    return true;
}

N* N48::getChild(uint8_t k) const{
    if(child_index[k] == empty_marker)
        return nullptr;
    return children[child_index[k]];
}

void N48::change(uint8_t key, N *val){
    children[child_index[key]] = val;
}
