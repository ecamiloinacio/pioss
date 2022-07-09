#ifndef MEMALLOC_H_
#define MEMALLOC_H_

#include <stddef.h>

void*
malloc_assert (size_t size);

void*
calloc_assert (size_t nmemb, size_t size);

#endif
