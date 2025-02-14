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
#include "h_frequency_table.h"

#include "../utils.h"

TEST_FUNCT(build_freq_table)
{
    struct h_pq *table = NULL;
    FILE *file = fopen("test.txt", "w+");
    fprintf(file, "aaabbc");
    rewind(file);

    const int result = build_freq_table(&table, &file, 1);

    fclose(file);
    remove("test.txt");

    CU_ASSERT(result > 0)
    CU_ASSERT(table['a'].priority == 3)
    CU_ASSERT(table['b'].priority == 2)
    CU_ASSERT(table['c'].priority == 1)

    free(table);
}

void runSuite(void)
{
    const CU_pSuite suite = CUnitCreateSuite("frequency_table_suite");
    if (suite) {
        ADD_SUITE_TEST(suite, build_freq_table)
    }
}
