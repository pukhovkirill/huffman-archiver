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

#include <stdlib.h>
#include <string.h>
#include "h_tree.h"
#include "h_priority_queue.h"
#include "h_stack.h"
#include "huffman.h"
#include "utils.h"

struct pair {
    char           code[256];
    struct h_tree  *node;
};

int h_pq_compare(const struct h_pq *a, const struct h_pq *b)
{
    return a->priority - b->priority;
}

struct h_tree *build_tree(const struct h_pq *f_table, const size_t f_size)
{
    h_priority_queue  *pq;
    struct h_pq       item1;
    struct h_pq       item2;
    struct h_pq       *n_item;

    pq = build_pq(f_table, f_size, &h_pq_compare);

    while(!is_left_alone(pq)) {
        item1 = pull_pq(pq, &h_pq_compare);
        item2 = pull_pq(pq, &h_pq_compare);

        n_item         = xcalloc("build_tree", 1, sizeof(*n_item));
        n_item->p_node = xcalloc("build_tree", 1, sizeof(*n_item->p_node));

        // We assign 1 to the symbol with a higher probability,
        // and 0 to the symbol with a lower probability
        // as elements of their codes.

        branch_switch:

        n_item->p_node->bit0 = item1.p_node;
        n_item->p_node->bit1 = item2.p_node;

        if(item1.priority > item2.priority) {
            struct h_pq temp;
            temp  = item2;
            item2 = item1;
            item1 = temp;
            goto branch_switch;
        }

        n_item->priority = item1.priority + item2.priority;

        offer_pq(pq, n_item, &h_pq_compare);
    }

    item1 = pull_pq(pq, &h_pq_compare);

    free(pq);
    return item1.p_node;
}

static void read_h_tree(char **t_table, struct h_tree *tree)
{
    h_stack        *stack;
    struct h_tree  *node;
    const size_t   s_capacity = 256;
    char           code[s_capacity];
    size_t         s_size;

    memset(code, 0, s_capacity*sizeof(*code));

    stack               = xcalloc("read_h_tree", 1, sizeof(*stack));
    struct pair *n_pair = xcalloc("read_h_tree", 1, sizeof(*n_pair));

    n_pair->node        = tree;
    memcpy(n_pair->code, "", 1);

    push_sck(stack, n_pair);

    while(stack->size > 0) {
        pop_sck(stack, (void **)&n_pair);
        node = n_pair->node;

        if(node != NULL) {
            if(node->bit0 == NULL && node->bit1 == NULL) {
                const int i = node->character;
                t_table[i]  = xcalloc("read_h_tree", s_capacity, sizeof(char));
                memcpy(t_table[i], n_pair->code, s_capacity);
            }
        }

        struct pair *b_pair;
        if(node->bit1 != NULL) {
            b_pair       = xcalloc("read_h_tree", 1, sizeof(*b_pair));
            b_pair->node = node->bit1;

            s_size = strlen(n_pair->code);
            memcpy(b_pair->code, n_pair->code, s_size);
            b_pair->code[s_size] = '1';

            push_sck(stack, b_pair);
        }

        if(node->bit0 != NULL) {
            b_pair       = xcalloc("read_h_tree", 1, sizeof(*b_pair));
            b_pair->node = node->bit0;

            s_size = strlen(n_pair->code);
            memcpy(b_pair->code, n_pair->code, s_size);
            b_pair->code[s_size] = '0';

            push_sck(stack, b_pair);
        }

        free(n_pair);
    }

    free(stack);
}

int h_codes_gen(char ***p_buf, struct h_tree *tree)
{
    char **table;

    table = xcalloc("h_codes_gen", TABLE_SIZE, sizeof(char *));
    read_h_tree(table, tree);

    *p_buf = table;
    return 0;
}

void free_tree(struct h_tree *tree)
{
    h_stack        *stack;
    struct h_tree  *node;

    stack = xcalloc("read_h_tree", 1, sizeof(*stack));
    push_sck(stack, tree);

    while(stack->size > 0) {
        pop_sck(stack, (void **)&node);

        if(node->bit0 != NULL)
            push_sck(stack, node->bit0);

        if(node->bit1 != NULL)
            push_sck(stack, node->bit1);

        free(node);
    }

    free(stack);
}

void free_codes(char **codes)
{
    for(int i = 0; i < TABLE_SIZE; i++) {
        if(codes[i] != NULL)
            free(codes[i]);
    }
    free(codes);
}
