#include "h_priority_queue.h"

#include <string.h>

#define TABLE_SIZE 256

static int proccess_file(FILE *file, struct h_pq *table, int t_size);
static void heapify(const struct h_pq *pq, int size);

struct h_pq *build_freq_table(const FILE *fls, const int count)
{
    int i;
    int ret;
    FILE file;
    struct h_pq *table;

    table = malloc(TABLE_SIZE * sizeof(*table));

    if(table == NULL) {
        perror("func_(h_priority_queue/build_freq_table): Cannot allocate memory, 'table' is NULL");
        return NULL;
    }

    memset(table, 0, TABLE_SIZE * sizeof(*table));

    for(i = 0; i < count; ++i) {
        file = *(fls+i);

        ret = proccess_file(&file, table, TABLE_SIZE);

        if(ret < 0) {
            perror("Failed to proccessing file");
            return NULL;
        }
    }

    return table;
}

int build_pq(const struct h_pq *table)
{

}

static struct h_pq *resize(struct h_pq *table, int *t_size, const int req_index)
{
    int size;
    struct h_pq *new_table;

    if(table == NULL) {
        perror("func_(h_priority_queue/resize): 'table' argument is NULL");
        return NULL;
    }

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

static int proccess_file(FILE *file, struct h_pq *table, int t_size)
{
    int ct_size;
    uint8_t byte;

    ct_size = t_size;

    while((byte = fgetc(file)) != EOF) {
        if(ct_size <= (int)byte) {
            table = resize(table, &ct_size, byte);
            if(table == NULL) {
                return -1;
            }
        }

        table[(int)byte].character = byte;
        table[(int)byte].character += 1;
    }

    return 0;
}

static void heapify(const struct h_pq *pq, const int size)
{

}