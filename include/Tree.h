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

private:
  N *root;
  bool prefixMatch();
};

#endif
