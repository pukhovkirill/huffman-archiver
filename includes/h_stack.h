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

#ifndef H_STACK_H
#define H_STACK_H

#include <inttypes.h>

struct h_sck {
    void          *value;
    struct h_sck  *next;
};

typedef struct {
    struct h_sck  *head;
    uint64_t      size;
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
