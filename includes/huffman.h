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
    uint64_t     pq_size;
} h_priority_queue;

#endif //HUFFMAN_H
