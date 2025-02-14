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

#include "h_priority_queue.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define PQ_DEFAULT_CAPCITY 32

static int resize_pq(h_priority_queue *pq);
static void down_heapify_pq(const h_priority_queue *pq, uint64_t i,
                            int (*compare)(const struct h_pq *a, const struct h_pq *b));
static void up_heapify_pq(const h_priority_queue *pq, uint64_t i,
                          int (*compare)(const struct h_pq *a, const struct h_pq *b));
static void swap_pq(struct h_pq *a, struct h_pq *b);

static void free_pq(h_priority_queue *pq)
{
    size_t       t_size;
    struct h_pq  *table;

    t_size = pq->pq_capacity;
    table  = pq->pq_array;

    for(int i = 0; i < t_size; i++)
        if(table[i].p_node != NULL)
            free(table[i].p_node);

    free(pq->pq_array);
    free(pq);
}

h_priority_queue *build_pq(const struct h_pq *t_table, const size_t t_size,
    int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    struct h_pq       pq_item;
    h_priority_queue  *pq;

    pq = xmalloc("build_pq", sizeof(*pq));
    memset(pq, 0, sizeof(*pq));

    pq->pq_array    = xcalloc("build_pq", PQ_DEFAULT_CAPCITY, sizeof(*pq->pq_array));
    pq->pq_capacity = PQ_DEFAULT_CAPCITY;

    for(int i = 0; i < t_size; i++) {
        pq_item = t_table[i];
        if(pq_item.priority > 0) {
            offer_pq(pq, &pq_item, compare);
        }
    }

    return pq;
}

struct h_pq pull_pq(h_priority_queue *pq,
    int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    struct h_pq  res = {0};
    struct h_pq  *table;
    uint64_t     t_size;

    table   = pq->pq_array;
    t_size  = pq->pq_nnodes;

    if(table[0].bucket != NULL) {
        struct h_pq *node;
        node = &table[0];

        while(node->bucket->bucket != NULL) {
            node = node->bucket;
        }

        memcpy(&res, node->bucket, sizeof(res));
        free(node->bucket);
        node->bucket = NULL;

        return res;
    }

    memcpy(&res, &table[0], sizeof(res));
    swap_pq(&table[0], &table[t_size-1]);

    pq->pq_nnodes = t_size-1;

    down_heapify_pq(pq, 0, compare);

    return res;
}

int offer_pq(h_priority_queue *pq, const struct h_pq *item,
    int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    struct h_pq  *table;
    uint64_t     t_size;
    uint64_t     t_capacity;

    table      = pq->pq_array;
    t_size     = pq->pq_nnodes;
    t_capacity = pq->pq_capacity;

    if(t_size+1 > t_capacity) {
        resize_pq(pq);
        table      = pq->pq_array;
        t_size     = pq->pq_nnodes;
    }

    for(int i = 0; i < t_size; i++) {
        if(table[i].priority == item->priority) {
            struct h_pq *tmp = xcalloc("offer_pq", 1, sizeof(tmp));

            memcpy(tmp, &table[i], sizeof(*tmp));
            memcpy(&table[i], item, sizeof(*table));
            table[i].bucket = tmp;

            return t_capacity;
        }
    }

    memcpy(&table[t_size], item, sizeof(*table));
    up_heapify_pq(pq, t_size, compare);

    t_size  += 1;

    pq->pq_nnodes = t_size;

    return t_size;
}

int is_left_alone(const h_priority_queue *pq)
{
    if(pq->pq_nnodes == 1){
        if(pq->pq_array->bucket == NULL)
            return 1;
    }
    return 0;
}

int resize_pq(h_priority_queue *pq)
{
    struct h_pq  *table;
    uint64_t     t_capacity;

    table      = pq->pq_array;
    t_capacity = pq->pq_capacity;

    if(table == NULL) {
        if(t_capacity == 0) t_capacity = PQ_DEFAULT_CAPCITY;
        table = xcalloc("resize_pq",t_capacity, sizeof(*table));
    }else {
        t_capacity = t_capacity * 2;
        table = xrealloc("resize_pq", table, t_capacity*sizeof(*table));
    }

    pq->pq_array    = table;
    pq->pq_capacity = t_capacity;

    return t_capacity;
}

void down_heapify_pq(const h_priority_queue *pq, uint64_t i,
                     int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    uint64_t     l;
    uint64_t     r;
    uint64_t     idx;
    uint64_t     t_size;
    struct h_pq  *table;

    t_size = pq->pq_nnodes;
    table  = pq->pq_array;

    while(2*i+1 < t_size) {
        l = i*2+1;
        r = i*2+2;

        idx = l;

        if(r < t_size && compare(&table[r], &table[l]) < 0)
            idx = r;

        if(compare(&table[i], &table[idx]) <= 0)
            break;
        swap_pq(&table[i], &table[idx]);
        i = idx;
    }
}

void up_heapify_pq(const h_priority_queue *pq, uint64_t i,
                   int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    struct h_pq *table;

    table = pq->pq_array;

    while(i > 0 && compare(&table[i], &table[(i-1)/2]) < 0) {
        swap_pq(&table[i], &table[(i-1)/2]);
        i = (i-1)/2;
    }
}

inline void swap_pq(struct h_pq *a, struct h_pq *b)
{
    struct h_pq tmp;
    tmp = *a;
    *a  = *b;
    *b  = tmp;
}
