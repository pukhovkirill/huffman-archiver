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

struct h_pq *build_freq_table(const FILE *fls, int count);
int build_pq(struct h_pq *table, ssize_t t_size, int (*compare)(const struct h_pq *a, const struct h_pq *b));