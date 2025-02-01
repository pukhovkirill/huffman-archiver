#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "../../src/huffman/h_priority_queue.c"

#include "utils.h"

int h_pq_compare(const struct h_pq *a, const struct h_pq *b)
{
    return a->priority - b->priority;
}

TEST_FUNCT(resize) {
    struct h_pq *table = malloc(2 * sizeof(struct h_pq));
    size_t size = 2;

    table = resize(table, &size, 5);

    CU_ASSERT(size == 6);
    free(table);
}

TEST_FUNCT(proccess_file) {
    struct h_pq table[256] = {0};
    FILE *file = fopen("test.txt", "w+");
    fprintf(file, "abcabc");
    rewind(file);

    const int result = processing_file(file, table, 256);
    fclose(file);
    remove("test.txt");

    CU_ASSERT(result > 0);
    CU_ASSERT(table['a'].priority == 2);
    CU_ASSERT(table['b'].priority == 2);
    CU_ASSERT(table['c'].priority == 2);
}

TEST_FUNCT(heapify) {
    struct h_pq table[] = {
        {'a', 3},
        {'b', 1},
        {'c', 2},
        {'d', 3},
        {'e', 4},
        {'f', 6},
        {'g', 5}
    };

    heapify(table, 3, 0, &h_pq_compare);

    CU_ASSERT(table[0].priority <= table[1].priority);
    CU_ASSERT(table[0].priority <= table[2].priority);

    CU_ASSERT(table[1].priority <= table[3].priority);
    CU_ASSERT(table[1].priority <= table[4].priority);

    CU_ASSERT(table[2].priority <= table[5].priority);
    CU_ASSERT(table[2].priority <= table[6].priority);
}

TEST_FUNCT(swap) {
    struct h_pq a = {'x', 1};
    struct h_pq b = {'y', 2};

    swap(&a, &b);

    CU_ASSERT(a.character == 'y');
    CU_ASSERT(a.priority == 2);
    CU_ASSERT(b.character == 'x');
    CU_ASSERT(b.priority == 1);
}

void runSuite(void) {
    const CU_pSuite suite = CUnitCreateSuite("h_pq_internal_functions_suite");
    if (suite) {
        ADD_SUITE_TEST(suite, resize)
        ADD_SUITE_TEST(suite, proccess_file)
        ADD_SUITE_TEST(suite, heapify)
        ADD_SUITE_TEST(suite, swap)
    }
}