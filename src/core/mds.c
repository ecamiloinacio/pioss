#include "mds.h"

#include "file_dist.h"
#include "logger.h"
#include "memalloc.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct mds_st
{
  file_dist *file_dist;
  uint32_t **file_table;
  uint32_t stripe_width;
  uint32_t num_dts;
  uint32_t num_files;
} mds_st;

static mds_st state;

void
mds_init (const char *file_dist, uint32_t stripe_width, uint32_t num_files,
	  uint32_t num_dts, uint32_t rng_seed)
{
  assert(file_dist);

  if (strcmp (FD_RANDOM, file_dist) == 0)
    state.file_dist = &fd_random;
  else if (strcmp (FD_ROUNDROBIN, file_dist) == 0)
    state.file_dist = &fd_roundrobin;
  else if (strcmp (FD_N2R2, file_dist) == 0)
    state.file_dist = &fd_n2r2;
  else
    {
      logf (ERROR, "Unknown file distribution: %s", file_dist);
      return;
    }
  state.file_dist->init (rng_seed);

  state.file_table = malloc_assert (num_files * sizeof(uint32_t*));
  for (int i = 0; i < num_files; i++)
    state.file_table[i] = NULL;

  state.stripe_width = stripe_width;
  state.num_dts = num_dts;
  state.num_files = num_files;
}

uint32_t*
mds_open (uint32_t fid)
{
  if (!state.file_table[fid])
    state.file_table[fid] = state.file_dist->select_dts (state.num_dts,
							 state.stripe_width, state.num_files, fid);

  return state.file_table[fid];
}

void
mds_clean ()
{
  for (int i = 0; i < state.num_files; i++)
    free (state.file_table[i]);

  free (state.file_table);
}
