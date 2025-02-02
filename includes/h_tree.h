#ifndef H_TREE_H
#define H_TREE_H

#include <stdio.h>
#include <inttypes.h>

// Huffman tree node
struct h_tree {
    uint8_t character;
    struct h_tree *bit1;
    struct h_tree *bit0;
};

#endif //H_TREE_H

struct h_tree *build_tree(FILE **fls, int f_cnt);
