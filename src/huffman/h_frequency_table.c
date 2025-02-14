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

#include "h_frequency_table.h"
#include <stdint.h>
#include <string.h>
#include "utils.h"

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
