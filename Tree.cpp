Tree::Tree() : root(new N256(nullptr, 0)){
};

Tree::~Tree(){

}

void* Tree::lookup() const {

}


void Tree::insert(uint8_t[] key, void *value){
		N *node = nullptr;
		N *nextNode = root;
		N *parentNode = nullptr;
		uint8_t parentKey, nodeKey = 0;
		int level = 0;

		while (true) {
				parentNode = node;
				parentKey = nodeKey;
				node = nextNode;

				// int nextLevel = level;
				// nodeKey = key[level];
				// nextNode = N::getChild(nodeKey, node);

				if(prefixMatch(node, key, nextLevel, commonPrefix, remainPrefix)) {
						//continue;
				} else {
						// create new node to hold split results
						auto newNode = new N4(commonPrefix, nextLevel - level);

						// add node and val, node as children
						newNode->insert(k[nextLevel], N::setLeaf(val));
						newNode->insert(remainPrefix, node);

						// update parent node to point to new node
						N::change(parentNode, parentKey, newNode);

						return;
				}

				level = nextLevel;
				nodeKey = key[level];
				nextNode = N::getChild(node, nodeKey);
				if(nextNode == nullptr) {
						N::insertNode(node, key, N::setLeaf(value), parentKey, parentNode);
						return;
				}
				if(N::isLeaf(nextNode)) {
						// Get the key of leaf node
						leaf_key = "";
						int prefix_len = 0;
						// find split point
						while(leaf_key[level + prefix_len] == key[level + prefix_len])
								prefix_len++;
						auto n4 = new N4(key[level], prefix_len);
						n4->insert(key[level + prefix_len], N::setLeaf(val));
						n4->insert(leaf_key[level + prefix_leaf], nextNode);
						N::change(node, k[level-1], n4);
						return;
				}
				level++;
		}
}

void Tree::remove(uint8_t[] key, void *value){
		N *node = nullptr;
		N *nextNode = root;
		N *parentNode = nullptr;
		uint8_t parentKey, nodeKey = 0;
		int level = 0;

		while(true) {
				parentNode = node;
				parentKey = nodeKey;
				node = nextNode;

				if(prefixMatch(node, key, nextLevel, commonPrefix, remainPrefix)) {
						nodeKey = key[level];
						nextNode = N::getChild(nodeKey, node);
						if (nextNode == nullptr) {
								return;
						}
						if (N::isLeaf(nextNode)) {
								// the value of leaf != val
								if()
										return;
								// the value of leaf == val
								if(node->count == 1 && node != root) {

								} else {
										N::removeNode(node, key[level], parentNode, parentKey);
								}
						}

				} else {
						// no match, just return
						return;
				}
				level++;
		}
}


bool Tree::prefixMatch(N *node, uint8_t[] key,
                       int& level, uint8_t[] commonPrefix,
                       uint8_t[] remainPrefix){
		uint8_t *nodePrefix = node->prefix;
		int sizeKey = sizeof(key)/sizeof(*key);
		int i = 0;
		for(; i<node->prefix_len; i++) {
				if(level+i >= sizeKey || key[level+i] != nodePrefix[i]) {
						int cnt = 0;
						for(int j=i; j<node->prefix_len; j++) {
								remainPrefix[cnt] = nodePrefix[j];
								cnt += 1;
						}
						level += i;
						return false;
				}
				commonPrefix[i] = nodePrefix[i];
		}
		level += i;
		return true;
}
