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

#include "h_stack.h"
#include <stdlib.h>
#include "utils.h"

int push_sck(h_stack *stack, void *value)
{
    struct h_sck *tmp;

    tmp = xcalloc("push_sck", 1, sizeof(*tmp));

    tmp->next    = stack->head;
    tmp->value   = value;
    stack->head  = tmp;
    stack->size += 1;

    return stack->size;
}

int pop_sck(h_stack *stack, void **p_buf)
{
    struct h_sck *tmp;

    if(stack->head == NULL || stack->size == 0)
        return 0;

    *p_buf       = stack->head->value;
    tmp          = stack->head;
    stack->head  = tmp->next;
    stack->size -= 1;

    free(tmp);
    return stack->size;
}
