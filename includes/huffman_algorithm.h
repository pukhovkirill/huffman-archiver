#ifndef HUFFMAN_ALGORITHM_H
#define HUFFMAN_ALGORITHM_H

#include <stdio.h>
#include <inttypes.h>

#endif //HUFFMAN_ALGORITHM_H

void *create_huffman_archive(const FILE *fls, size_t f_cnt);
void *extract_huffman_archive(const FILE *achv);
void *append_to_huffman_archive(const FILE *achv, const FILE *fls, size_t f_cnt);
