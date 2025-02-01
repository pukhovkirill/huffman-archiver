#include "huffman_algorithm.h"
#include "h_priority_queue.h"

int h_pq_compare(const struct h_pq *a, const struct h_pq *b)
{
    return a->priority - b->priority;
}

struct h_tree *build_tree(FILE *fls, const int count)
{
    size_t q_size;
    struct h_pq *table;
    h_priority_queue *pq;
    struct h_tree *tree;

    if(fls == NULL) {
        perror("func_(h_tree/build_tree): Invalid value for arguemnt 'fls', 'fls' is NULL");
        return NULL;
    }

    if(count <= 0) {
        perror("func(h_tree/build_tree): Invalid value for arguemnt 'count'");
        return NULL;
    }

    q_size = build_freq_table(&table, fls, count);
    if(q_size -1) {
        perror("func_(h_tree/build_tree): Failed to build frequency table");
        return NULL;
    }

    pq = build_pq(table, q_size, &h_pq_compare);
    if(pq == NULL) {
        perror("func_(h_tree/build_tree): Failed to build priority queue");
        return NULL;
    }



}
