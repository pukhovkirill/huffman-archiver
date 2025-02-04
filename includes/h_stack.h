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

/**
 *
 * @param stack - pointer to stack
 * @param value - pointer to new value, witch will be added
 * @return size of stack or -1
 *
 * default push to stack
 *
 * stack structure
 *  ------         ------         ------
 * |  3  |  --->  |  2  |  --->  |  1  |
 * ------         ------         ------
 *
 *                 ------
 *  push new -->  |  4  |
 *                ------
 *
 *  ------        ------         ------         ------
 * |  4  |  -->  |  3  |  --->  |  2  |  --->  |  1  |
 * ------        ------         ------         ------
 */
int push_sck(h_stack *stack, void *value);

/**
 *
 * @param stack - pointer to stack
 * @param p_buf - pointer to a buffer, which will be filled with the stored value
 * @return size of stack or -1
 *
 * default pop from stack
 *
 * stack structure
 *  ------        ------         ------         ------
 * |  4  |  -->  |  3  |  --->  |  2  |  --->  |  1  |
 * ------        ------         ------         ------
 *
 *                 ------
 *  pop elem -->  |  4  |
 *                ------
 *
 *  ------         ------         ------
 * |  3  |  --->  |  2  |  --->  |  1  |
 * ------         ------         ------
 */
int pop_sck(h_stack *stack, void **p_buf);
