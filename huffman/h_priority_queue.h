#ifndef H_PRIORITY_QUEUE_H
#define H_PRIORITY_QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#endif //H_PRIORITY_QUEUE_H

// Priority queue node
struct h_pq {
    uint8_t character;
    uint64_t priority;
};

/**
 *
 * @param t_table - destination buffer when frequency table will be saved
 * @param fls  - array of files, witch will be processed
 * @param count - count of files
 * @return size of frequency table;
 *
 * function linearly counts the number
 * of each unique character from files
 */
int build_freq_table(struct h_pq **t_table, FILE *fls, int count);

/**
 * 
 * @param table - pointer to frequency table created by `build_freq_table` function
 * @param t_size - size of table
 * @param compare - comparator for `h_pq` struct
 *
 *  binary heap
 *       1      <-- min
 *      |\
 *     |  \
 *    2    3    <-- large than root
 *   | \  | \
 *  4  5  6  7  <-- large than roots
 *
 * function build priority queue (based on binary heap)
 * in the same array `table`
 */
void build_pq(struct h_pq *table, ssize_t t_size,
 int (*compare)(const struct h_pq *a, const struct h_pq *b));

/**
 *
 * @param queue - pointer to priority queue
 * @param t_size - pointer to size of queue (size is decreasing)
 * @param compare - comparator for `h_pq` struct
 * @return first element of queue
 *
 * function pulls root from queue
 * and heapifying remain nodes
 *
 *           1  <-- min              7
 *          |\                      |\
 *         |  \                    |  \
 *        2    3    ---------->   2    3
 *       | \  | \                | \  | \
 *      4  5  6 |7|             4  5  6 |1|  <-- delete
 *
 *                  heapify
 *      ->  7                        2
 *         |\                       |\
 *        |  \                     |  \
 * min-> |2|  3     ---------->   7    3
 *       | \  |                  | \   |
 *      4  5  6                 4  5   6
 *
 *                  heapify
 *            2                      2  <-- done
 *           |\                     |\
 *          |  \                   |  \
 *     ->  7   3    ---------->   4    3
 *        | \  |                 | \   |
 * min-> |4| 5 6                7  5   6
 */
struct h_pq get_next_item(struct h_pq *queue, ssize_t *t_size,
    int (*compare)(const struct h_pq *a, const struct h_pq *b));