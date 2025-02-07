#include "h_stack.h"

int push_sck(h_stack *stack, void *value)
{
    struct h_sck *tmp;

    tmp = xcalloc("push_sck", 1, sizeof(*tmp));

    tmp->next = stack->head;
    tmp->value = value;

    stack->head = tmp;
    stack->size += 1;

    return stack->size;
}

int pop_sck(h_stack *stack, void **p_buf)
{
    struct h_sck *tmp;

    if(stack->head == NULL || stack->size == 0) {
        return 0;
    }

    tmp = stack->head;

    stack->head = tmp->next;
    stack->size -= 1;

    *p_buf = tmp->value;

    free(tmp);
    return stack->size;
}
