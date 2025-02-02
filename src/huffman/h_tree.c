#include "h_tree.h"
#include "h_priority_queue.h"

int h_pq_compare(const struct h_pq *a, const struct h_pq *b)
{
    return a->priority - b->priority;
}

struct h_tree *build_tree(FILE **fls, const int f_cnt)
{
    size_t q_size;
    struct h_pq *table;
    h_priority_queue *pq;
    struct h_pq item1;
    struct h_pq item2;
    struct h_pq *n_item;

    if(fls == NULL) {
        perror("func_(h_tree/build_tree): Invalid value for arguemnt 'fls', 'fls' is NULL");
        return NULL;
    }

    if(f_cnt <= 0) {
        perror("func(h_tree/build_tree): Invalid value for arguemnt 'count'");
        return NULL;
    }

    q_size = build_freq_table(&table, fls, f_cnt);

    if(q_size == -1) {
        perror("func_(h_tree/build_tree): Failed to build frequency table");
        return NULL;
    }

    pq = build_pq(table, q_size, &h_pq_compare);

    free(table);

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
