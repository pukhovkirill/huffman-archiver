#ifndef H_PRIORITY_QUEUE_H
#define H_PRIORITY_QUEUE_H

#include <inttypes.h>
#include <stdio.h>

#endif //H_PRIORITY_QUEUE_H

// Priority queue node
struct h_pq {
    uint8_t character;
    uint64_t priority;
};

extern int (*compare)(const int *a, const int *b);

struct h_pq *build_freq_table(const FILE *fls);
int build_pq(const struct h_pq *table);
