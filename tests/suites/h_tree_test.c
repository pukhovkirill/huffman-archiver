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
#include "h_tree.h"

#include "../utils.h"

TEST_FUNCT(build_tree)
{
    struct h_pq *buf;
    struct h_tree *tree;
    FILE *file = fopen("test.txt", "w+");
    fprintf(file, "aaabbc");
    rewind(file);

    const int f_size = build_freq_table(&buf, &file, 1);
    tree = build_tree(buf, f_size);

    fclose(file);
    remove("test.txt");

    CU_ASSERT(tree != NULL)
    CU_ASSERT(tree->bit0->character == 'a')
    CU_ASSERT(tree->bit1->bit1->character == 'b')
    CU_ASSERT(tree->bit1->bit0->character == 'c')

    free(buf);
    free(tree->bit1);
    free(tree->bit0->bit0);
    free(tree->bit0->bit1);
    free(tree->bit0);
    free(tree);
}

TEST_FUNCT(h_codes_gen)
{
    struct h_pq *f_buf;
    struct h_tree *tree;
    FILE *file = fopen("test.txt", "w+");
    fprintf(file, "aaabbc");
    rewind(file);

    const int f_size = build_freq_table(&f_buf, &file, 1);
    tree = build_tree(f_buf, f_size);

    char **buf;

    const int res = h_codes_gen(&buf, tree);

    fclose(file);
    remove("test.txt");

    CU_ASSERT(res == 0)
    CU_ASSERT(buf != NULL)
    CU_ASSERT(strcmp(buf['a'], "0")  == 0)
    CU_ASSERT(strcmp(buf['b'], "11") == 0)
    CU_ASSERT(strcmp(buf['c'], "10") == 0)

    free(tree->bit1);
    free(tree->bit0->bit0);
    free(tree->bit0->bit1);
    free(tree->bit0);
    free(tree);

    free(f_buf);
    free(buf['a']);
    free(buf['b']);
    free(buf['c']);
    free(buf);
}

void runSuite(void)
{
    const CU_pSuite suite = CUnitCreateSuite("tree_suite");
    if (suite) {
        ADD_SUITE_TEST(suite, build_tree)
        ADD_SUITE_TEST(suite, h_codes_gen)
    }
}