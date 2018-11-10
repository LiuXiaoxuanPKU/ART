//
//  N.cpp
//  xiaoxuan
//
//  Created by MAC on 2018/10/25.
//  Copyright © 2018 xiaoxuan. All rights reserved.
//

#include "N.h"

#include "N4.cpp"
#include "N16.cpp"
#include "N48.cpp"
#include "N256.cpp"

#include <iostream>
using namespace std;

template<class curN, class biggerN>
void N::insertGrow(curN *n, uint8_t key, N *val, uint8_t key_par, N *node_par){
	// current node is not full
	if(n->insert(key,val))
		return;

	// initialize a bigger node
	auto big_node = new biggerN(n->prefix, n->prefix_len);
	// copy original keys and children
	n->copyTo(big_node);
	// insert key,val to the new node
	big_node->insert(key,val);
	// replace old node with new node
	N::change(node_par, key_par, big_node);
	
	// delete old node
	delete n;
}

template<class curN, class smallerN>
void N::removeAndShrink(curN *n, uint8_t key, uint8_t key_par, N *parentNode){

	// initialize a smaller node
	auto nSmall = new smallerN(n->prefix, n->prefix_len);
	// remove key
	n->remove(key);
	// copy to smaller node
	n->copyTo(nSmall);
	// replace old node with new node
	N::change(parentNode, key_par, nSmall);
}

void N::change(N *node, uint8_t key, N *val){
	switch (node->type) {
	case NTypes::N4: {
		N4* n = static_cast<N4 *>(node);
		n->change(key, val);
		return;
	}
	case NTypes::N16: {
		N16* n = static_cast<N16 *>(node);
		n->change(key, val);
		return;
	}
	case NTypes::N48: {
		N48* n = static_cast<N48 *>(node);
		n->change(key, val);
		return;
	}
	case NTypes::N256: {
		N256* n = static_cast<N256 *>(node);
		n->change(key, val);
		return;
	}
	default:
		break;
	}
}

void N::insertNode(N *node, N *parentNode, uint8_t keyParent, uint8_t key, N *val){
	switch (node->type) {
	case NTypes::N4: {
		auto n = static_cast<N4 *>(node);
		insertGrow<N4, N16>(n, key, val, keyParent, parentNode);
		return;
	}
	case NTypes::N16: {
		auto n = static_cast<N16 *>(node);
		insertGrow<N16, N48>(n, key, val, keyParent, parentNode);
		return;
	}
	case NTypes::N48: {
		auto n = static_cast<N48 *>(node);
		insertGrow<N48, N256>(n, key, val, keyParent, parentNode);
		return;
	}
	case NTypes::N256: {
		auto n = static_cast<N256 *>(node);
		n->insert(key, val);
		return;
	}
	}
	cout << "[Error]insert and grow Node" << endl;
	assert(false);
}

void N::removeNode(N *node, N *parentNode, uint8_t key_par, uint8_t key){
	switch (node->type) {
	case NTypes::N4: {
		auto n = static_cast<N4 *>(node);
		n->remove(key);
		return;
	}
	case NTypes::N16: {
		auto n = static_cast<N16 *>(node);
		removeAndShrink<N16, N4>(n, key, key_par, parentNode);
		return;
	}
	case NTypes::N48: {
		auto n = static_cast<N48 *>(node);
		removeAndShrink<N48, N16>(n, key, key_par, parentNode);
		return;
	}
	case NTypes::N256: {
		auto n = static_cast<N256 *>(node);
		removeAndShrink<N256, N48>(n, key, key_par, parentNode);
		return;
	}
	}
	cout << "[Error]remove and shrink Node" << endl;
	assert(false);
}
