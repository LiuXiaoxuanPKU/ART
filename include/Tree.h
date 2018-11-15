#ifndef TREE_H
#define TREE_H

#include "N.h"

class Tree{
public:
  Tree();
  void* lookup(const uint8_t *key);
  void insert(const uint8_t *key, void* val);
  void remove(const uint8_t *key);
  ~Tree();

  static bool prefixMatch(N *node,uint8_t *key, int sizeKey,
                   int &keyLevel, int &nodeLevel,
                   uint8_t *commonPrefix);
  static int getSize(uint8_t *key);


private:
  N *root;
};

#endif
