#ifndef H_PRIORITY_QUEUE_H
#define H_PRIORITY_QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"

#endif //H_PRIORITY_QUEUE_H

/**
 *
 * @param t_table - pointer to frequency table
 * @param t_size - size of table
 * @param compare - comparator for `h_pq` struct
 * @return pointer to priority queue
 *
 *  binary heap
 *       1      <-- min
 *      | \
 *     |   \
 *    2     3    <-- large than root
 *   | \   | \
 *  4   5 6   7  <-- large than roots
 *
 * function build priority queue (based on binary heap)
 * in the same array `table`
 */
h_priority_queue *build_pq(const struct h_pq *t_table, size_t t_size,
    int (*compare)(const struct h_pq *a, const struct h_pq *b));

/**
 *
 * @param pq - pointer to priority queue
 * @param compare - comparator for `h_pq` struct
 * @return root of binary heap
 *
 * function pulls root from binary heap
 * and heapifying remain nodes (from top to bottom)
 *
 *              1  <-- min               7
 *             | \                      | \
 *            |   \                    |   \
 *           2     3    ---------->   2     3
 *          | \   | \                | \   | \
 *         4   5 6  |7|             4   5 6  |1|  <-- delete
 *
 *                     heapify
 *          ->  7                        2
 *             | \                      | \
 *            |   \                    |   \
 *    min-> |2|    3    ---------->   7     3
 *          | \   | \                | \   | \
 *         4   5 6                  4   5 6
 *
 *                     heapify
 *              2                       2  <-- done
 *             | \                     | \
 *            |   \                   |   \
 *       ->  7     3    ---------->  4     3
 *          | \   | \               | \   | \
 *  min-> |4|  5 6                 7   5 6
 */
struct h_pq pull_pq(h_priority_queue *pq,
    int (*compare)(const struct h_pq *a, const struct h_pq *b));

/**
 *
 * @param pq - pointer to priority queue
 * @param item - pointer to the element to be added
 * @param compare - comparator for `h_pq` struct
 * @return return current priority queue size or -1
 *
 * function adds an element to the binary heap
 * the last layer is filled from left to right without “holes”
 * and heapifying nodes (from bottom to top)
 *
 *             2                               2
 *            | \                             | \
 *           |   \                           |   \
 *          4     3       ---------->       4     3
 *         | \   | \                       | \   | \
 *        7   5 6  |_|  <-- space         7   5 6   1 <-- new element
 *
 *                        heapify
 *             2                               2
 *            | \                             | \
 *           |   \                           |   \
 *          4    |3|  <-  ---------->       7     1
 *         | \   | \                       | \   | \
 *        7   5 6   1 <-- less than root  4   5 6   3
 *
 *                        heapify
 *            |2|  <-                          1  <-- done
 *            | \                             | \
 *           |   \                           |   \
 *          2     1  <-- less than root     7     2
 *         | \   | \                       | \   | \
 *        4   5 6   3                     4   5 6   3
 */
int offer_pq(h_priority_queue *pq, const struct h_pq *item,
    int (*compare)(const struct h_pq *a, const struct h_pq *b));
