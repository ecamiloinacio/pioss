#include "dts.h"

#include "memalloc.h"

#include <stdlib.h>

typedef struct dts_st
{
  uint32_t num_dts;
  uint64_t *bytes_stored;
} dts_st;

static dts_st state;

void
dts_init (uint32_t num_dts)
{
  state.num_dts = num_dts;
  state.bytes_stored = calloc_assert (num_dts, sizeof(uint64_t));
}

void
dts_store (uint32_t dtsind, uint64_t bytes)
{
  state.bytes_stored[dtsind] += bytes;
}

void
dts_clean ()
{
  free (state.bytes_stored);
}

dts_results*
dts_get_results ()
{
  dts_results *results = malloc_assert (sizeof(dts_results));
  results->num_dts = state.num_dts;
  results->bytes_stored = state.bytes_stored;

  return results;
}
