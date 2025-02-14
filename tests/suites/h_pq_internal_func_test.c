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

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "huffman/h_priority_queue.c"

#include "../utils.h"

int h_pq_compare(const struct h_pq *a, const struct h_pq *b)
{
    return a->priority - b->priority;
}

TEST_FUNCT(resize_pq)
{
    struct h_pq *table;
    h_priority_queue *pq;
    const uint64_t capacity = 4;

    table = malloc(2 * sizeof(*table));
    pq = calloc(1, sizeof(*pq));

    pq->pq_array = table;
    pq->pq_capacity = capacity;

    const int test_capacity = resize_pq(pq);

    CU_ASSERT(test_capacity == capacity * 2)
    CU_ASSERT(pq->pq_array != NULL)

    free(pq->pq_array);
    free(pq);
}

TEST_FUNCT(resize_pq_when_table_is_NULL)
{
    h_priority_queue *pq;
    const uint64_t capacity = 4;

    pq = calloc(1, sizeof(*pq));
    pq->pq_capacity = capacity;

    const int test_capacity = resize_pq(pq);

    CU_ASSERT(test_capacity == capacity)
    CU_ASSERT(pq->pq_array != NULL)

    free(pq->pq_array);
    free(pq);
}

TEST_FUNCT(resize_pq_when_table_is_NULL_and_capacity_is_0)
{
    h_priority_queue *pq;
    const uint64_t capacity = PQ_DEFAULT_CAPCITY;

    pq = calloc(1, sizeof(*pq));

    const int test_capacity = resize_pq(pq);

    CU_ASSERT(test_capacity == capacity)
    CU_ASSERT(pq->pq_array != NULL)

    free(pq->pq_array);
    free(pq);
}

TEST_FUNCT(down_heapify_pq)
{
    struct h_pq table[] = {
        {(struct h_tree *){0}, 7},
        {(struct h_tree *){0}, 3},
        {(struct h_tree *){0}, 4},
        {(struct h_tree *){0}, 5},
        {(struct h_tree *){0}, 6},
        {(struct h_tree *){0}, 9},
        {(struct h_tree *){0}, 8}
    };

    h_priority_queue *pq = calloc(1, sizeof(*pq));
    pq->pq_array = table;
    pq->pq_capacity = 7;
    pq->pq_nnodes = 7;

    down_heapify_pq(pq, 0, &h_pq_compare);

    CU_ASSERT(table[0].priority <= table[1].priority)
    CU_ASSERT(table[0].priority <= table[2].priority)

    CU_ASSERT(table[1].priority <= table[3].priority)
    CU_ASSERT(table[1].priority <= table[4].priority)

    CU_ASSERT(table[2].priority <= table[5].priority)
    CU_ASSERT(table[2].priority <= table[6].priority)

    free(pq);
}

TEST_FUNCT(up_heapify_pq)
{
    struct h_pq table[] = {
        {(struct h_tree *){0}, 2},
        {(struct h_tree *){0}, 3},
        {(struct h_tree *){0}, 4},
        {(struct h_tree *){0}, 5},
        {(struct h_tree *){0}, 6},
        {(struct h_tree *){0}, 7},
        {(struct h_tree *){0}, 1}
    };

    h_priority_queue *pq = calloc(1, sizeof(*pq));
    pq->pq_array = table;
    pq->pq_capacity = 7;
    pq->pq_nnodes = 7;

    up_heapify_pq(pq, 6, &h_pq_compare);

    CU_ASSERT(table[0].priority <= table[1].priority)
    CU_ASSERT(table[0].priority <= table[2].priority)

    CU_ASSERT(table[1].priority <= table[3].priority)
    CU_ASSERT(table[1].priority <= table[4].priority)

    CU_ASSERT(table[2].priority <= table[5].priority)
    CU_ASSERT(table[2].priority <= table[6].priority)

    free(pq);
}

TEST_FUNCT(swap_pq)
{

    struct h_tree t_a;
    struct h_tree t_b;

    struct h_pq a = {&t_a, 1};
    struct h_pq b = {&t_b, 2};

    swap_pq(&a, &b);

    CU_ASSERT(a.p_node == &t_b)
    CU_ASSERT(a.priority == 2)

    CU_ASSERT(b.p_node == &t_a)
    CU_ASSERT(b.priority == 1)
}

void runSuite(void)
{
    const CU_pSuite suite = CUnitCreateSuite("h_pq_internal_functions_suite");
    if (suite) {
        ADD_SUITE_TEST(suite, resize_pq)
        ADD_SUITE_TEST(suite, resize_pq_when_table_is_NULL)
        ADD_SUITE_TEST(suite, resize_pq_when_table_is_NULL_and_capacity_is_0)
        ADD_SUITE_TEST(suite, down_heapify_pq)
        ADD_SUITE_TEST(suite, up_heapify_pq)
        ADD_SUITE_TEST(suite, swap_pq)
    }
}