#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "h_priority_queue.h"

#include "utils.h"

int h_pq_compare(const struct h_pq *a, const struct h_pq *b)
{
    return a->priority - b->priority;
}


TEST_FUNCT(build_freq_table) {
    struct h_pq *table = NULL;
    FILE *file = fopen("test.txt", "w+");
    fprintf(file, "aaabbc");
    rewind(file);

    const int result = build_freq_table(&table, file, 1);
    fclose(file);
    remove("test.txt");

    CU_ASSERT(result > 0);
    CU_ASSERT(table['a'].priority == 3);
    CU_ASSERT(table['b'].priority == 2);
    CU_ASSERT(table['c'].priority == 1);

    free(table);
}

TEST_FUNCT(build_pq) {
    struct h_pq table[] = {
        {'a', 3}, {'b', 2}, {'c', 1}
    };

    build_pq(table, 3, h_pq_compare);
    CU_ASSERT(table[0].priority <= table[1].priority);
}

TEST_FUNCT(get_next_item) {
    struct h_pq *table;
    table = calloc(3, sizeof(*table));

    for(int i = 0; i < 3; i++) {
        table[i].character = 'a'+i;
        table[i].priority = i+1;
    }

    size_t size = 3;

    const struct h_pq item = get_next_item(table, &size, h_pq_compare);
    CU_ASSERT(item.character == 'a');
    CU_ASSERT(item.priority == 1);
    CU_ASSERT(size == 2);
}

void runSuite(void) {
    const CU_pSuite suite = CUnitCreateSuite("Suite1");
    if (suite) {
        ADD_SUITE_TEST(suite, build_freq_table)
        ADD_SUITE_TEST(suite, build_pq)
        ADD_SUITE_TEST(suite, get_next_item)
    }
}