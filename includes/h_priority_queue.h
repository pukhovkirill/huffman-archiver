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

#ifndef H_PRIORITY_QUEUE_H
#define H_PRIORITY_QUEUE_H

#include <stdio.h>
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

int is_left_alone(const h_priority_queue *pq);
