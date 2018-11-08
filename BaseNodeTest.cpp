//
//  Ntest.cpp
//  xiaoxuan
//
//  Created by MAC on 2018/10/25.
//  Copyright © 2018 xiaoxuan. All rights reserved.
//

#include <stdio.h>
#include "N.h"
#include <iostream>
#include <assert.h>

using namespace std;

void testN4(){
	N4 testN4({},0);
	N4 child1({},0);
	N4 child2({},0);
	N4 child3({},0);
	N4 child4({},0);
	N4 child5({},0);
	testN4.insert(1, &child1);
	testN4.insert(2, &child2);
	testN4.insert(3, &child3);
	testN4.insert(4, &child4);
	assert(testN4.insert(5, &child5) == false);
	assert(testN4.getChild(1)==&child1);
	assert(testN4.getChild(4)==&child4);
	assert(testN4.remove(1)==true);
	assert(testN4.remove(3)==true);
	assert(testN4.getChild(1)==nullptr);
}

void testN48(){
	uint8_t rootprefix[2] = {200,201};
	N48 root(rootprefix,2);
	N* c[50];
	for(int i=0; i<48; i++) {
		c[i] = new N48({},0);
		root.insert(i, c[i]);
	}
	c[48] = new N48({},0);
	c[49] = new N48({},0);
	assert(root.insert(48, c[48])==false);
	assert(root.remove(1)==true);
	assert(root.getChild(1)==nullptr);
	assert(root.getChild(2)==c[2]);
	for(int i=0; i<50; i++)
		delete c[i];
}

void testN256(){

}

void testN(){
	uint8_t rootprefix[2] = {200,201};
	N4 parent({},0);
	N4 node(rootprefix,2);
	// bad code
	uint8_t parent_key = -1;
	parent.insert(parent_key,&node);

	N4* children[50];
	for(int i=0; i<48; i++) {
		children[i] = new N4({},0);
		N::insertNode(&node, &parent,parent_key, (uint8_t)i, children[i]);
	}
	assert(node.type == NTypes::N48);
}



int main(){
	testN4();
	cout << "finish test N4" << endl;
	testN48();
	cout << "finish test N48" << endl;
	testN256();
	cout << "======finish basic Node test======" << endl;

	//  testN();
	cout << "finish grow test" << endl;
	return 0;
}
