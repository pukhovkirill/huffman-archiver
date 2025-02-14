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

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <inttypes.h>

#define TABLE_SIZE 256

// Huffman tree node
struct h_tree {
    struct h_tree  *bit1;
    struct h_tree  *bit0;
    uint8_t        character;
};

// Priority queue node
struct h_pq {
    struct h_tree  *p_node;
    uint64_t       priority;
    struct h_pq    *bucket;
};

typedef struct {
    struct h_pq  *pq_array;
    uint64_t     pq_capacity;
    uint64_t     pq_nnodes;
} h_priority_queue;

#endif //HUFFMAN_H
