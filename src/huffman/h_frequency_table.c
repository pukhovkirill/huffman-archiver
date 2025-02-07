#include "h_frequency_table.h"

static void processing_file(FILE *file, struct h_pq *table);

int build_freq_table(struct h_pq **p_buf, FILE **fls, const int f_cnt)
{
    struct h_pq *table;

    table = xmalloc("build_freq_table", TABLE_SIZE * sizeof(*table));
    memset(table, 0, TABLE_SIZE * sizeof(*table));

    for(int i = 0; i < f_cnt; ++i) {
        processing_file(fls[i], table);
    }

    *p_buf = table;
    return TABLE_SIZE;
}

void processing_file(FILE *file, struct h_pq *table)
{
    struct h_tree *node;

    int byte;
    while((byte = fgetc(file)) != EOF) {
        table[byte].priority += 1;

        if(table[byte].p_node == NULL) {
            node = xcalloc("processing_file", 1, sizeof(*node));
            node->character = (uint8_t) byte;
            table[byte].p_node = node;
        }
    }
    rewind(file);
}
