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

/**
 * 
 * @param fls  - array of files, witch will be processed
 * @param f_cnt - count of files
 * @return pointer to root of huffman tree or NULL
 *
 * function build huffman tree for 'f_cnt' files.
 *
 * assume that we have the file 'file.txt' with content below:
 * $ cat file.txt
 * aaabbc
 * $
 *
 * frequency table:
 * a - 3
 * b - 2
 * c - 1
 *
 * the huffman tree for this file will look like:
 *
 *            1
 * |a|3|---------------|
 *                     |
 *       0             |---|_|6|
 * |b|2|---|     0     |
 *         |---|_|3|---|
 *       1 |
 * |c|3|---|
 */
struct h_tree *build_tree(FILE **fls, int f_cnt);

/**
 * 
 * @param p_buf - pointer to a buffer, which will be filled by code matrix
 * @param tree - pointer to root of huffman tree
 * @return 0 or -1
 *
 * function read huffman tree and create code matrix
 *
 * if the huffman tree look like:
 *
 *            1
 * |a|3|---------------|
 *                     |
 *       0             |---|_|6|
 * |b|2|---|     0     |
 *         |---|_|3|---|
 *       1 |
 * |c|3|---|
 *
 * then table:
 *  ch   code
 *    ...
 *  a ->  1
 *  b -> 00
 *  c -> 01
 *   ...
 */
int h_codes_gen(char ***p_buf, struct h_tree *tree);
