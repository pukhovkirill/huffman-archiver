#ifndef H_FREQUENCY_TABLE_H
#define H_FREQUENCY_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"
#include "utils.h"

#endif //H_FREQUENCY_TABLE_H

/**
 *
 * @param p_buf - pointer to destination buffer when frequency table will be saved
 * @param fls  - array of files, witch will be processed
 * @param f_cnt - count of files
 * @return size of frequency table or -1
 *
 * function linearly counts the number
 * of each unique character from files
 */
int build_freq_table(struct h_pq **p_buf, FILE **fls, int f_cnt);
