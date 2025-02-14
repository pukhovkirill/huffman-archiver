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

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#endif //UTILS_H

/**
 *
 * @param place - name of the function where allocation is requested
 * @param s - size of memory to allocate
 * @return pointer to allocated memory or terminates program
 *
 * function allocates 's' bytes of memory using malloc().
 *
 * if allocation fails, the function prints an error message
 * with the name of the function 'place' where the error occurred
 * and terminates the program.
 */
void *xmalloc(const char *place, const size_t s);

/**
 *
 * @param place - name of the function where allocation is requested
 * @param n - number of elements to allocate
 * @param s - size of each element
 * @return pointer to allocated memory or terminates program
 *
 * function allocates memory for an array of 'n' elements of 's' bytes each
 * using calloc().
 *
 * if allocation fails, the function prints an error message
 * with the name of the function 'place' where the error occurred
 * and terminates the program.
 */
void *xcalloc(const char *place, const size_t n, const size_t s);

/**
 *
 * @param place - name of the function where reallocation is requested
 * @param ptr - pointer to previously allocated memory block
 * @param s - new size of memory block
 * @return pointer to reallocated memory or terminates program
 *
 * function changes the size of the memory block pointed to by 'ptr' to 's' bytes
 * using realloc().
 *
 * if reallocation fails, the function prints an error message
 * with the name of the function 'place' where the error occurred
 * and terminates the program.
 */
void *xrealloc(const char *place, void *ptr, const size_t s);
