#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "h_stack.h"

#include "../utils.h"

TEST_FUNCT(push_n_pop_sck)
{
    h_stack *stack;
    char value1 = '1';
    char value2 = '2';
    char value3 = '3';

    stack = calloc(1, sizeof(*stack));

    push_sck(stack, &value1);
    push_sck(stack, &value2);
    push_sck(stack, &value3);

    CU_ASSERT(stack->size == 3)
    CU_ASSERT(stack->head != NULL)
    CU_ASSERT(stack->head->value != NULL)
    CU_ASSERT(*(char *)stack->head->value == '3')

    char *test_value;

    pop_sck(stack, (void **)&test_value);

    CU_ASSERT(stack->size == 2)
    CU_ASSERT(*test_value == '3')
    CU_ASSERT(stack->head != NULL)
    CU_ASSERT(*(char *)stack->head->value == '2')

    pop_sck(stack, (void **)&test_value);

    CU_ASSERT(stack->size == 1)
    CU_ASSERT(*test_value == '2')
    CU_ASSERT(stack->head != NULL)
    CU_ASSERT(*(char *)stack->head->value == '1')

    pop_sck(stack, (void **)&test_value);

    CU_ASSERT(stack->size == 0)
    CU_ASSERT(*test_value == '1')
    CU_ASSERT(stack->head == NULL)

    free(stack);
}

void runSuite(void)
{
    const CU_pSuite suite = CUnitCreateSuite("stack_suite");
    if (suite) {
        ADD_SUITE_TEST(suite, push_n_pop_sck)
    }
}