#include "h_tree.h"
#include "h_priority_queue.h"
#include "h_stack.h"

struct pair {
    char code;
    struct h_tree *node;
};

int h_pq_compare(const struct h_pq *a, const struct h_pq *b)
{
    return a->priority - b->priority;
}

struct h_tree *build_tree(const struct h_pq *f_table, const size_t f_size)
{
    h_priority_queue *pq;
    struct h_pq item1;
    struct h_pq item2;
    struct h_pq *n_item;

    pq = build_pq(f_table, f_size, &h_pq_compare);

    while(pq->pq_size > 1) {
        item1 = pull_pq(pq, &h_pq_compare);
        item2 = pull_pq(pq, &h_pq_compare);

        n_item = xcalloc("build_tree", 1, sizeof(*n_item));
        n_item->p_node = xcalloc("build_tree", 1, sizeof(*n_item->p_node));

        // We assign 1 to the symbol with a higher probability,
        // and 0 to the symbol with a lower probability
        // as elements of their codes.

        branch_switch:

        n_item->p_node->bit0 = item1.p_node;
        n_item->p_node->bit1 = item2.p_node;

        if(item1.priority > item2.priority) {
            const struct h_pq temp = item2;
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
    h_stack *stack;
    struct h_tree *node;

    size_t s_size = 0;
    const size_t s_capacity = 256;
    char code[s_capacity];
    memset(code, 0, s_capacity*sizeof(*code));

    stack = xcalloc("read_h_tree", 1, sizeof(*stack));

    struct pair *n_pair = xcalloc("read_h_tree", 1, sizeof(*n_pair));
    n_pair->node = tree;

    push_sck(stack, n_pair);

    while(stack->size > 0) {
        pop_sck(stack, (void **) &n_pair);
        node = n_pair->node;

        const char ch = n_pair->code;
        if(ch != 0) code[s_size++] = ch;

        free(n_pair);

        if(node != NULL) {
            if(node->bit0 == NULL && node->bit1 == NULL) {
                t_table[node->character] =
                    xcalloc("read_h_tree", s_capacity, sizeof(char));
                memcpy(t_table[node->character], code, s_capacity);
                code[--s_size] = 0;
            }
        }

        struct pair *b_pair;
        if(node->bit1 != NULL) {
            b_pair = xcalloc("read_h_tree", 1, sizeof(*b_pair));
            b_pair->node = node->bit1;
            b_pair->code = '1';
            push_sck(stack, b_pair);
        }

        if(node->bit0 != NULL) {
            b_pair = xcalloc("read_h_tree", 1, sizeof(*b_pair));
            b_pair->node = node->bit0;
            b_pair->code = '0';
            push_sck(stack, b_pair);
        }
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

}

void free_codes(char **codes)
{

}
