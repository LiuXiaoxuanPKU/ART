//
//  N4.cpp
//  xiaoxuan
//
//  Created by MAC on 2018/10/25.
//  Copyright © 2018 xiaoxuan. All rights reserved.
//

#include "N.h"
#include <assert.h>
#include <iostream>
using namespace std;

bool N4::insert(uint8_t k, N* node){
	if(count == 4)
		return false;
    int i = 0;
	while(i < count && k >= keys[i])
		i += 1;
	memcpy(keys + i + 1, keys + i, (count - i) * sizeof(k));
	memcpy(children + i + 1, children + i, (count - i) * sizeof(node));
	keys[i] = k;
	children[i] = node;
	count += 1;
	return true;
}

bool N4::remove(uint8_t k){
	for(int i = 0; i < count; i++) {
		if(keys[i]==k) {
			memcpy(keys + i, keys + i + 1, (count - i - 1) * sizeof(k));
			memcpy(children + i, children + i + 1, (count - i - 1) * sizeof(N*));
			count--;
			return true;
		}
	}
	cout << "[Error]Remove N4 element not exist" << endl;
	assert(false);
}

void N4::change(uint8_t key, N *val){
	for(uint8_t i = 0; i < count; i++) {
		if(children[i] != nullptr && keys[i] == key) {
			children[i] = val;
			return;
		}
	}
}

N* N4::getChild(uint8_t k){
	for(int i = 0; i < count; i++) {
		if(this->keys[i]==k){
			return children[i];
		}
	}
	return nullptr;
}

template<class NODE>
void N4::copyTo(NODE *n) const {
	for (int i = 0; i < count; i++) {
		n->insert(keys[i],children[i]);
	}
}
