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
