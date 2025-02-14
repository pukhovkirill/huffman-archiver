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