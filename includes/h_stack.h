#ifndef H_STACK_H
#define H_STACK_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

struct h_sck {
    void *value;
    struct h_sck *next;
};

typedef struct {
    struct h_sck *head;
    uint64_t size;
} h_stack;

#endif //H_STACK_H

int push_sck(h_stack *stack, void *value);
int pop_sck(h_stack *stack, void **p_buf);
