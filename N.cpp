//
//  N.cpp
//  xiaoxuan
//
//  Created by MAC on 2018/10/25.
//  Copyright © 2018 xiaoxuan. All rights reserved.
//

#include "N.hpp"

template<typename curN, typename biggerN>
void N::insertGrow(curN *n, uint8_t k, N *node, uint8_t key_par, N* node_par){
    // current node is not full
    if(n->insert(k,node))
        return;

    // initialize a bigger node
    auto big_node = new biggerN(n->prefix, n->prefix_len);
    // copy original keys and children
    n->copyTo(big_node);
    // insert key,val to the new node
    big_node->insert(k,node);
    // replace old node with new node
    change(node_par, key_par, big_node);
    // delete old node
    delete n;
}

template<typename curN, typename biggerN>
void N::removeAndShrink(curN* n, uint8_t k){

}

void N::change(N *node, uint8_t key, N *val){
    switch (node->type) {
        case NTypes::N4:{
            N4* n = static_cast<N4 *>(node);
            n->change(key, val);
            return;
        }
        case NTypes::N16:{
            N16* n = static_cast<N16 *>(node);
            n->change(key, val);
            return;
        }
        case NTypes::N48: {
            N48* n = static_cast<N48 *>(node);
            n->change(key, val);
            return;
        }
        case NTypes::N256:{
            N256* n = static_cast<N256 *>(node);
            n->change(key, val);
            return;
        }
        default:
            break;
    }
}

void N::insertNode()
