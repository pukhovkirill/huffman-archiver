#include "h_frequency_table.h"

static int processing_file(FILE *file, struct h_pq *table);

int build_freq_table(struct h_pq **p_buf, FILE **fls, const int f_cnt)
{
    int i;
    struct h_pq *table;
    int ret = TABLE_SIZE;

    table = malloc(TABLE_SIZE * sizeof(*table));

    if(table == NULL) {
        perror("func_(h_frequency_table/build_freq_table): Cannot allocate memory, 'table' is NULL");
        return -1;
    }

    memset(table, 0, TABLE_SIZE * sizeof(*table));

    for(i = 0; i < f_cnt; ++i) {
        ret = processing_file(fls[i], table);
        if(ret < 0) {
            return -1;
        }
    }

    *p_buf = table;
    return ret;
}

int processing_file(FILE *file, struct h_pq *table)
{
    int byte;
    while((byte = fgetc(file)) != EOF) {
        table[byte].priority += 1;

        if(table[byte].p_node == NULL) {
            struct h_tree *node = calloc(1, sizeof(*node));

            if(node == NULL) {
                perror("func_(h_frequency_table/processing_file): Cannot allocate memory, 'node' is NULL");
                return -1;
            }

            node->character = (uint8_t) byte;

            table[byte].p_node = node;
        }
    }

    return TABLE_SIZE;
}
