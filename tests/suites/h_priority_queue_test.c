#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "h_priority_queue.h"

#include "../utils.h"

int h_pq_compare(const struct h_pq *a, const struct h_pq *b)
{
    return a->priority - b->priority;
}

TEST_FUNCT(build_pq)
{
    const size_t t_size = 3;
    struct h_pq *table;
    table = calloc(t_size, sizeof(*table));

    for(int i = 0; i < t_size; i++) {
        table[i].p_node = (struct h_tree *){0};
        table[i].priority = i+1;
    }

    h_priority_queue *pq = build_pq(table, t_size, h_pq_compare);

    CU_ASSERT(pq->pq_array != NULL);
    CU_ASSERT(pq->pq_capacity == 32);
    CU_ASSERT(pq->pq_nnodes == 3);
    CU_ASSERT(table[0].priority <= table[1].priority);
    CU_ASSERT(table[1].priority <= table[2].priority);

    free(table);
    free(pq);
}

TEST_FUNCT(build_pq_with_zeros)
{
    const size_t t_size = 7;
    struct h_pq *table;
    table = calloc(t_size, sizeof(*table));

    for(int i = 1; i < 4; i++) {
        table[i].p_node = (struct h_tree *){0};
        table[i].priority = i;
    }

    h_priority_queue *pq = build_pq(table, t_size, h_pq_compare);

    CU_ASSERT(pq->pq_array != NULL);
    CU_ASSERT(pq->pq_capacity == 32);
    CU_ASSERT(pq->pq_nnodes == 3);
    CU_ASSERT(table[0].priority <= table[1].priority);
    CU_ASSERT(table[1].priority <= table[2].priority);

    free(table);
    free(pq);
}

TEST_FUNCT(pull_pq)
{
    const uint64_t size = 3;
    const uint64_t capacity = 4;
    struct h_pq *table;
    h_priority_queue *pq;

    table = calloc(size, sizeof(*table));
    for(int i = 0; i < size; i++) {
        table[i].p_node = (struct h_tree *){0};
        table[i].priority = i+1;
    }

    pq = calloc(capacity, sizeof(*pq));
    pq->pq_array = table;
    pq->pq_capacity = capacity;
    pq->pq_nnodes = 3;

    const struct h_pq item = pull_pq(pq, h_pq_compare);

    CU_ASSERT(item.priority == 1);

    free(table);
    free(pq);
}

TEST_FUNCT(offer_pq)
{
    const uint64_t size = 3;
    const uint64_t capacity = 4;
    struct h_pq *table;
    h_priority_queue *pq;

    table = calloc(capacity, sizeof(*table));
    for(int i = 0; i < size; i++) {
        table[i].p_node = (struct h_tree *){0};
        table[i].priority = i+1;
    }

    pq = calloc(1, sizeof(*pq));
    pq->pq_array = table;
    pq->pq_capacity = capacity;
    pq->pq_nnodes = 3;

    struct h_pq item;
    item.p_node = (struct h_tree *){0};
    item.priority = 5;

    const int test_size = offer_pq(pq, &item,h_pq_compare);

    CU_ASSERT(pq->pq_capacity == 4);
    CU_ASSERT(test_size == pq->pq_nnodes);

    free(table);
    free(pq);
}

void runSuite(void)
{
    const CU_pSuite suite = CUnitCreateSuite("priority_queue_suite");
    if (suite) {
        ADD_SUITE_TEST(suite, build_pq)
        ADD_SUITE_TEST(suite, build_pq_with_zeros)
        ADD_SUITE_TEST(suite, pull_pq)
        ADD_SUITE_TEST(suite, offer_pq)
    }
}