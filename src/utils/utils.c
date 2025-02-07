#include "utils.h"

void *xmalloc(const char *place, const size_t s)
{
    void *p = malloc(s);
    if(p == NULL) {
        fprintf(stderr, "%s: Cannot allocate memory (xmalloc(%zu)).\n", place, s);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *xcalloc(const char *place, const size_t n, const size_t s)
{
    void *p = calloc(n, s);
    if(p == NULL) {
        fprintf(stderr, "%s: Cannot allocate memory (xcalloc(%zu, %zu)).\n", place, n, s);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *xrealloc(const char *place, void *ptr, const size_t s)
{
    void *p = realloc(ptr, s);
    if(p == NULL) {
        fprintf(stderr, "%s: Cannot realloc memory (xrealloc(%zu)).\n", place, s);
        exit(EXIT_FAILURE);
    }
    return p;
}
