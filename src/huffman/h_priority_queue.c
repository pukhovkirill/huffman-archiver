#include <h_priority_queue.h>

#define PQ_DEFAULT_CAPCITY 32

static int resize_pq(h_priority_queue *pq);
static void down_heapify_pq(const h_priority_queue *pq, uint64_t i,
                            int (*compare)(const struct h_pq *a, const struct h_pq *b));
static void up_heapify_pq(const h_priority_queue *pq, uint64_t i,
                          int (*compare)(const struct h_pq *a, const struct h_pq *b));
static void swap_pq(struct h_pq *a, struct h_pq *b);

static void free_pq(h_priority_queue *pq)
{
    size_t t_size;
    struct h_pq *table;

    t_size = pq->pq_capacity;
    table = pq->pq_array;

    for(int i = 0; i < t_size; i++)
        if(table[i].p_node != NULL)
            free(table[i].p_node);

    free(pq->pq_array);
    free(pq);
}

h_priority_queue *build_pq(const struct h_pq *t_table, const size_t t_size,
    int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    struct h_pq pq_item;
    h_priority_queue *pq;

    pq = malloc(sizeof(*pq));

    if(pq == NULL) {
        perror("func_(h_priority_queue/build_pq): Cannot allocate memory, 'pq' is NULL");
        return NULL;
    }

    memset(pq, 0, sizeof(*pq));

    pq->pq_array = calloc(PQ_DEFAULT_CAPCITY, sizeof(*pq->pq_array));
    pq->pq_capacity = PQ_DEFAULT_CAPCITY;

    if(pq->pq_array == NULL) {
        perror("func_(h_priority_queue/build_pq): Cannot allocate memory, 'pq->pq_array' is NULL");
        free(pq);
        return NULL;
    }

    for(int i = 0; i < t_size; i++) {
        pq_item = t_table[i];
        if(pq_item.priority == 0)
            continue;
        if(offer_pq(pq, &pq_item, compare) == -1) {
            perror("func_(h_priority_queue/build_pq): Failed to add item in priority queue");
            free_pq(pq);
            return NULL;
        }
    }

    return pq;
}

struct h_pq pull_pq(h_priority_queue *pq,
    int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    struct h_pq res = {0};
    struct h_pq *table;
    uint64_t t_size;

    table = pq->pq_array;
    t_size = pq->pq_size;

    memcpy(&res, &table[0], sizeof(res));
    swap_pq(&table[0], &table[t_size-1]);
    pq->pq_size = t_size-1;

    down_heapify_pq(pq, 0, compare);

    return res;
}

int offer_pq(h_priority_queue *pq, const struct h_pq *item,
    int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    struct h_pq *table;
    uint64_t t_size;
    uint64_t t_capacity;

    table = pq->pq_array;
    t_size = pq->pq_size;
    t_capacity = pq->pq_capacity;

    if(t_size+1 > t_capacity) {
        if(resize_pq(pq) == -1) {
            perror("func_(h_priority_queue/offer_pq): Cannot resize priority queue");
            return -1;
        }
    }

    memcpy(&table[t_size], item, sizeof(*table));
    up_heapify_pq(pq, t_size, compare);
    t_size += 1;

    pq->pq_size = t_size;

    return t_size;
}

int resize_pq(h_priority_queue *pq)
{
    struct h_pq *table;
    uint64_t t_capacity;

    table = pq->pq_array;
    t_capacity = pq->pq_capacity;

    if(table == NULL) {
        if(t_capacity == 0) t_capacity = PQ_DEFAULT_CAPCITY;
        table = calloc(t_capacity, sizeof(*table));
    }else {
        t_capacity = t_capacity * 2;
        table = realloc(table, t_capacity*sizeof(*table));
    }

    if(table == NULL) {
        perror("func_(h_priority_queue/resize_pq): Cannot allocate memory, 'table' is NULL");
        return -1;
    }

    pq->pq_array = table;
    pq->pq_capacity = t_capacity;

    return t_capacity;
}

void down_heapify_pq(const h_priority_queue *pq, uint64_t i,
                     int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    uint64_t l;
    uint64_t r;
    uint64_t idx;
    uint64_t t_size;
    struct h_pq *table;

    t_size = pq->pq_size;
    table = pq->pq_array;

    while(2*i+1 < t_size) {
        l = i*2+1;
        r = i*2+2;

        idx = l;

        if(r < t_size && compare(&table[r], &table[l]) < 0)
            idx = r;

        if(compare(&table[i], &table[idx]) <= 0)
            break;
        swap_pq(&table[i], &table[idx]);
        i = idx;
    }
}

void up_heapify_pq(const h_priority_queue *pq, uint64_t i,
                   int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    struct h_pq *table;

    table = pq->pq_array;

    while(i > 0 && compare(&table[i], &table[(i-1)/2]) < 0) {
        swap_pq(&table[i], &table[(i-1)/2]);
        i = (i-1)/2;
    }
}

inline void swap_pq(struct h_pq *a, struct h_pq *b)
{
    struct h_pq tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
