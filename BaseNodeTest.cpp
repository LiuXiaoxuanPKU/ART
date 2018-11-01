//
//  Ntest.cpp
//  xiaoxuan
//
//  Created by MAC on 2018/10/25.
//  Copyright © 2018 xiaoxuan. All rights reserved.
//

#include <stdio.h>
#include "N.hpp"
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
    N48* children[50];
    for(int i=0;i<48;i++){
        children[i] = new N48({},0);
        root.insert(i, children[i]);
    }
    children[48] = new N48({},0);
    children[49] = new N48({},0);
    assert(root.insert(48, children[48])==false);
    assert(root.remove(1)==true);
    assert(root.getChild(1)==nullptr);
    assert(root.getChild(2)==children[2]);
    for(int i=0;i<50;i++)
        delete children[i];
}

void testN256(){
    
}

void testN(){
    uint8_t rootprefix[2] = {200,201};
    N4 node(rootprefix,2);
    N4* children[50];
    for(int i=0;i<48;i++){
        children[i] = new N4({},0);
        node.insert(i, children[i]);
    }
    assert(node.type == NTypes::N48);
}


int main(){
    testN4();
    testN48();
    testN256();
    cout << "finish basic class test" << endl;
    testN();
    cout << "finish grow test" << endl;
    
}
