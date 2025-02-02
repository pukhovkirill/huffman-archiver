#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "h_tree.h"

#include "utils.h"

TEST_FUNCT(build_tree)
{
    struct h_tree *tree;
    FILE *file = fopen("test.txt", "w+");
    fprintf(file, "aaabbc");
    rewind(file);

    tree = build_tree(&file, 1);

    fclose(file);
    remove("test.txt");

    CU_ASSERT(tree != NULL);
    CU_ASSERT(tree->bit1->character == 'a');
    CU_ASSERT(tree->bit0->bit0->character == 'c');
    CU_ASSERT(tree->bit0->bit1->character == 'b');

    free(tree->bit1);
    free(tree->bit0->bit0);
    free(tree->bit0->bit1);
    free(tree->bit0);
    free(tree);
}

void runSuite(void)
{
    const CU_pSuite suite = CUnitCreateSuite("tree_suite");
    if (suite) {
        ADD_SUITE_TEST(suite, build_tree)
    }
}