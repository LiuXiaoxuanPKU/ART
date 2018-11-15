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
#include "N.h"
using namespace std;

bool N16::insert(uint8_t k, N* node){
	if(count == 16)
		return false;
	// TODO: use __mm__cmplt to speed up
	unsigned int i = 0;
	while(i < count && k >= keys[i])
		i += 1;
	memcpy(keys + i + 1, keys + i, (count - i) * sizeof(k));
	memcpy(children + i + 1, children + i, (count - i) * sizeof(node));
	keys[i] = k;
	children[i] = node;
	count += 1;
	return true;
}

N* N16::getChild(uint8_t k){
	for(int i=0; i<count; i++) {
		if(this->keys[i]==k)
			return children[i];
	}
	return nullptr;
}

bool N16::remove(uint8_t k){
	// if we need shrink, return false
	if(count == 4)
		return false;
	
	for(int i = 0; i < count; i++) {
		if(keys[i]==k) {
			memcpy(keys + i, keys + i + 1, (count - i - 1) * sizeof(k));
			memcpy(children + i, children + i + 1, (count - i - 1) * sizeof(N*));
			count--;
			return true;
		}
	}
	cout << " [Error]Remove N16 element not exist" << endl;
	assert(false);
}

void N16::change(uint8_t key, N *val){
	for(int i = 0; i < count; i++) {
		if(keys[i] == key) {
			children[i] = val;
			return;
		}
	}
}

template<class NODE>
void N16::copyTo(NODE* n) const {
	for(int i = 0; i < count; i++) {
		n->insert(keys[i], children[i]);
	}
}
