#include "memalloc.h"

#include <assert.h>
#include <stdlib.h>

void*
malloc_assert (size_t size)
{
  void *ptr = malloc (size);
  assert(ptr);
  return ptr;
}

void*
calloc_assert (size_t nmemb, size_t size)
{
  void *ptr = calloc (nmemb, size);
  assert(ptr);
  return ptr;
}
