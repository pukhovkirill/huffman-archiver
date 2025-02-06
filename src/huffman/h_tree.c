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

    if(pq == NULL) {
        perror("func_(h_tree/build_tree): Failed to build priority queue");
        return NULL;
    }

    while(pq->pq_size > 1) {
        item1 = pull_pq(pq, &h_pq_compare);
        item2 = pull_pq(pq, &h_pq_compare);

        n_item = calloc(1, sizeof(*n_item));
        n_item->p_node = calloc(1, sizeof(*n_item->p_node));

        if(n_item == NULL) {
            perror("func_(h_tree/build_tree): Cannot allocate memory, 'n_item' is NULL");
            return NULL;
        }

        // We assign 1 to the symbol with a higher probability,
        // and 0 to the symbol with a lower probability
        // as elements of their codes.

        branch_switch:

        n_item->p_node->bit1 = item1.p_node;
        n_item->p_node->bit0 = item2.p_node;

        if(item2.priority > item1.priority) {
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

static int read_h_tree(char **t_table, struct h_tree *tree)
{
    h_stack *stack;
    struct h_tree *node;

    size_t s_size = 0;
    const size_t s_capacity = 256;
    char code[s_capacity];

    memset(code, 0, s_capacity*sizeof(*code));

    stack = calloc(1, sizeof(*stack));

    if(stack == NULL) {
        perror("func_(h_tree/read_h_tree): Cannot allocate memory, 'stack' is NULL");
        return -1;
    }

    struct pair *n_pair = calloc(1, sizeof(*n_pair));
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
                t_table[node->character] = calloc(s_capacity, sizeof(char));

                if(t_table[node->character] == NULL) {
                    perror("func_(h_tree/read_h_tree): Cannot allocate memory, 'table' is NULL");
                    return -1;
                }

                memcpy(t_table[node->character], code, s_capacity);
                code[--s_size] = 0;
            }
        }

        if(node->bit0 != NULL) {
            struct pair *b_pair;

            b_pair = calloc(1, sizeof(*b_pair));
            b_pair->node = node->bit0;
            b_pair->code = '0';

            push_sck(stack, b_pair);
        }

        if(node->bit1 != NULL) {
            struct pair *b_pair;

            b_pair = calloc(1, sizeof(*b_pair));
            b_pair->node = node->bit1;
            b_pair->code = '1';

            push_sck(stack, b_pair);
        }
    }

    free(stack);
    return 0;
}

int h_codes_gen(char ***p_buf, struct h_tree *tree)
{
    size_t t_size;
    char **table;
    int ret;

    t_size = TABLE_SIZE;
    table = calloc(t_size, sizeof(char *));

    if(table == NULL) {
        perror("func_(h_tree/h_codes_gen): Cannot allocate memory, 'table' is NULL");
        return -1;
    }

    ret = read_h_tree(table, tree);

    *p_buf = table;
    return ret;
}
