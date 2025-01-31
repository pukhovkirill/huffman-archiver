#ifndef H_TREE_H
#define H_TREE_H

#include <inttypes.h>
#include <stdio.h>

#endif //H_TREE_H

// Huffman tree node
struct h_tree {
    uint8_t is_code;
    uint8_t value;
    struct h_tree *left;
    struct h_tree *right;
};

struct h_tree *build_tree(const FILE *f);
