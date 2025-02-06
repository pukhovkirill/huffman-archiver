#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "h_frequency_table.h"

#include "utils.h"

TEST_FUNCT(build_freq_table)
{
    struct h_pq *table = NULL;
    FILE *file = fopen("test.txt", "w+");
    fprintf(file, "aaabbc");
    rewind(file);

    const int result = build_freq_table(&table, &file, 1);

    fclose(file);
    remove("test.txt");

    CU_ASSERT(result > 0);
    CU_ASSERT(table['a'].priority == 3);
    CU_ASSERT(table['b'].priority == 2);
    CU_ASSERT(table['c'].priority == 1);

    free(table);
}

void runSuite(void)
{
    const CU_pSuite suite = CUnitCreateSuite("frequency_table_suite");
    if (suite) {
        ADD_SUITE_TEST(suite, build_freq_table)
    }
}
