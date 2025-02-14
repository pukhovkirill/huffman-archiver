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

#include "utils.h"
#include <stdlib.h>

void *xmalloc(const char *place, const size_t s)
{
    void *p = malloc(s);
    if(p == NULL) {
        fprintf(stderr, "%s: Cannot allocate memory (xmalloc(%zu))\n", place, s);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *xcalloc(const char *place, const size_t n, const size_t s)
{
    void *p = calloc(n, s);
    if(p == NULL) {
        fprintf(stderr, "%s: Cannot allocate memory (xcalloc(%zu, %zu))\n", place, n, s);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *xrealloc(const char *place, void *ptr, const size_t s)
{
    void *p = realloc(ptr, s);
    if(p == NULL) {
        fprintf(stderr, "%s: Cannot realloc memory (xrealloc(%zu))\n", place, s);
        exit(EXIT_FAILURE);
    }
    return p;
}
