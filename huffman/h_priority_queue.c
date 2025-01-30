#include "h_priority_queue.h"

#include <string.h>

#define TABLE_SIZE 256

#ifndef UNIT_TEST

static struct h_pq *resize(struct h_pq *table, int *t_size, int req_index);
static int proccess_file(FILE *file, struct h_pq *table, int t_size);
static void heapify(struct h_pq *table, ssize_t t_size, uint32_t i,
          int (*compare)(const struct h_pq *a, const struct h_pq *b));

#else //UNIT_TEST

struct h_pq *resize(struct h_pq *table, int *t_size, int req_index);
int proccess_file(FILE *file, struct h_pq *table, int t_size);
void heapify(struct h_pq *table, ssize_t t_size, uint32_t i);

#endif //UNIT_TEST

int build_freq_table(struct h_pq **t_table, FILE *fls, int count)
{
    int i;
    struct h_pq *table;
    int ret = TABLE_SIZE;

    if(count <= 0) {
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

    for(i = 0; i < count; ++i) {

        ret = proccess_file(fls+i, table, TABLE_SIZE);

        if(ret < 0) {
            perror("func_(h_priority_queue/build_freq_table): Failed to proccessing file");
            return -1;
        }
    }

    *t_table = table;
    return ret;
}

static void swap(struct h_pq *a, struct h_pq *b)
{
    struct h_pq tmp = *a;
    *a = *b;
    *b = tmp;
}

inline void heapify(
    struct h_pq *table,
    const ssize_t t_size,
    uint32_t i,
    int (*compare)(const struct h_pq *a, const struct h_pq *b))
{
    uint32_t l;
    uint32_t r;
    uint32_t idx;

    while(2*i+1 < t_size) {
        l = i*2+1;
        r = i*2+2;

        idx = l;

        if(r < t_size && compare(&table[r], &table[l]) < 0)
            idx = r;

        if(compare(&table[i], &table[idx]) <= 0)
            break;
        swap(table+i, table+idx);
        i = idx;
    }
}

void build_pq(
    struct h_pq *table,
    const ssize_t t_size,
    int (*compare)(const struct h_pq *a, const struct h_pq *b))
{

    if(table == NULL) {
        perror("func_(h_priority_queue/build_pq): 'table' argument is NULL");
        return;
    }

    if(compare == NULL) {
        perror("func_(h_priority_queue/build_pq): 'compare' argument is NULL");
        return;
    }

    for (int i = t_size-1; i >= 0; --i) {
        heapify(table, t_size, i, compare);
    }
}

struct h_pq *resize(struct h_pq *table, int *t_size, const int req_index)
{
    int size;
    struct h_pq *new_table;

    size = *t_size;

    new_table = malloc((req_index+1)*sizeof(*new_table));

    if(new_table == NULL) {
        perror("func_(h_priority_queue/resize): Cannot allocate memory, 'new_table' is NULL");
        return NULL;
    }

    memset(&new_table, 0, (req_index+1)*sizeof(*new_table));

    *t_size = req_index+1;

    for(int i = 0; i < size; i++) {
        new_table[i] = table[i];
    }

    free(table);
    return new_table;
}

int proccess_file(FILE *file, struct h_pq *table, const int t_size)
{
    int byte;
    int ct_size;

    ct_size = t_size;
    while((byte = fgetc(file)) != EOF) {
        if(ct_size <= byte) {
            table = resize(table, &ct_size, byte);
            if(table == NULL) {
                return -1;
            }
        }

        table[byte].character = (uint8_t) byte;
        table[byte].priority += 1;
    }

    return ct_size;
}