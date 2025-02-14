/*
Copyright (c) 2025 Kirill Pukhov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef H_TREE_H
#define H_TREE_H

#include <huffman.h>

#endif //H_TREE_H

/**
 * 
 * @param f_table - pointer to frequency table
 * @param f_size - size of frequency table
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
struct h_tree *build_tree(const struct h_pq *f_table, size_t f_size);

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

/**
 *
 * @param tree - pointer to root of huffman tree
 *
 * function free memory allocated for huffman tree
 *
 * the function uses a non-recursive Depth-First Search (DFS)
 * algorithm with an explicit stack to traverse and free
 * all nodes of the tree.
 */
void free_tree(struct h_tree *tree);


/**
 *
 * @param codes - pointer to code matrix
 *
 * function free memory allocated for code matrix
 * all allocated memory for storing these codes
 * will be released
 */
void free_codes(char **codes);
