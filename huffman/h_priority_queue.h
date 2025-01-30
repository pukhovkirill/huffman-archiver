#ifndef H_PRIORITY_QUEUE_H
#define H_PRIORITY_QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#endif //H_PRIORITY_QUEUE_H

// Priority queue node
struct h_pq {
    uint8_t character;
    uint64_t priority;
};

int build_freq_table(struct h_pq **t_table, FILE *fls, int count);
void build_pq(struct h_pq *table, ssize_t t_size, int (*compare)(const struct h_pq *a, const struct h_pq *b));