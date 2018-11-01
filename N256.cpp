//
//  N256.cpp
//  xiaoxuan
//
//  Created by MAC on 2018/10/25.
//  Copyright © 2018 xiaoxuan. All rights reserved.
//

#include <stdio.h>
#include "N.hpp"

bool N256::insert(uint8_t k, N* n){
    children[k] = n;
    count++;
    return true;
}

bool N256::remove(uint8_t k){
    children[k] = nullptr;
    count--;
    return true;
}

N* N256::getChild(uint8_t k) const{
    return children[k];
}

void N256::change(uint8_t key, N *val){
    children[key] = val;
}



