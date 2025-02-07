#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>

#endif //UTILS_H

void *xmalloc(const char *place, size_t s);
void *xcalloc(const char *place, size_t n, size_t s);
void *xrealloc(const char *place, void *ptr, size_t s);
