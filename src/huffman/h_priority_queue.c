#include <h_priority_queue.h>

#include <string.h>

#define TABLE_SIZE 256
#define PQ_DEFAULT_CAPCITY 32

static int processing_file(FILE *file, struct h_pq *table);

static int resize_pq(h_priority_queue *pq);
static void down_heapify_pq(const h_priority_queue *pq, uint64_t i,
                            int (*compare)(const struct h_pq *a, const struct h_pq *b));
static void up_heapify_pq(const h_priority_queue *pq, uint64_t i,
                          int (*compare)(const struct h_pq *a, const struct h_pq *b));
static void swap_pq(struct h_pq *a, struct h_pq *b);

int build_freq_table(struct h_pq **t_table, FILE **fls, const int f_cnt)
{
    int i;
    struct h_pq *table;
    int ret = TABLE_SIZE;

    if(f_cnt <= 0) {
        perror("func_(h_priority_queue/build_freq_table): Invalid value for arguemnt 'count'");
        return -1;
    }

    if(fls == NULL) {
        perror("func_(h_priority_queue/build_freq_table): Invalid value for arguemnt 'fls', 'fls' is NULL");
        return -1;
    }

    table = malloc(TABLE_SIZE * sizeof(*table));

    if(table == NULL) {
        perror("func_(h_priority_queue/build_freq_table): Cannot allocate memory, 'table' is NULL");
        return -1;
    }

    memset(table, 0, TABLE_SIZE * sizeof(*table));

    for(i = 0; i < f_cnt; ++i) {
        ret = processing_file(fls[i], table);
        if(ret < 0) {
            return -1;
        }
    }

    *t_table = table;
    return ret;
}

h_priority_queue *build_pq(const struct h_pq *t_table, const size_t t_size,
    int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    struct h_pq pq_item;
    h_priority_queue *pq;

    if(t_table == NULL) {
        perror("func_(h_priority_queue/build_pq): Invalid value for arguemnt 'table', 'table' is NULL");
        return NULL;
    }

    if(t_size <= 0) {
        perror("func_(h_priority_queue/build_pq): Invalid value for arguemnt 't_size'");
        return NULL;
    }

    if(compare == NULL) {
        perror("func_(h_priority_queue/build_pq): Invalid value for arguemnt 'compare', 'compare' is NULL");
        return NULL;
    }

    pq = malloc(sizeof(*pq));
    memset(pq, 0, sizeof(*pq));

    if(pq == NULL) {
        perror("func_(h_priority_queue/build_freq_table): Cannot allocate memory, 'pq' is NULL");
        return NULL;
    }

    pq->pq_array = calloc(PQ_DEFAULT_CAPCITY, sizeof(*pq->pq_array));
    pq->pq_capacity = PQ_DEFAULT_CAPCITY;

    if(pq->pq_array == NULL) {
        perror("func_(h_priority_queue/build_freq_table): Cannot allocate memory, 'pq->pq_array' is NULL");
        return NULL;
    }

    for(int i = 0; i < t_size; i++) {
        pq_item = t_table[i];
        if(pq_item.priority == 0)
            continue;
        if(offer_pq(pq, &pq_item, compare) == -1) {
            perror("func_(h_priority_queue/build_pq): Failed to add item in priority queue");
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

    if(pq == NULL) {
        perror("func_(h_priority_queue/pull_pq): Invalid value for arguemnt 'pq', 'pq' is NULL");
        return res;
    }

    if(compare == NULL) {
        perror("func_(h_priority_queue/pull_pq): Invalid value for arguemnt 'compare', 'compare' is NULL");
        return res;
    }

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

    if(pq == NULL) {
        perror("func_(h_priority_queue/offer_pq): Invalid value for arguemnt 'pq', 'pq' is NULL");
        return -1;
    }

    if(item == NULL) {
        perror("func_(h_priority_queue/offer_pq): Invalid value for arguemnt 'item', 'item' is NULL");
        return -1;
    }

    if(compare == NULL) {
        perror("func_(h_priority_queue/offer_pq): Invalid value for arguemnt 'compare', 'compare' is NULL");
        return -1;
    }

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

int processing_file(FILE *file, struct h_pq *table)
{
    int byte;
    while((byte = fgetc(file)) != EOF) {
        table[byte].priority += 1;

        if(table[byte].p_node == NULL) {
            struct h_tree *node = calloc(1, sizeof(*node));

            if(node == NULL) {
                perror("func_(h_priority_queue/processing_file): Cannot allocate memory, 'node' is NULL");
                return -1;
            }

            node->character = (uint8_t) byte;

            table[byte].p_node = node;
        }
    }

    return TABLE_SIZE;
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
