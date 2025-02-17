#include "file_dist.h"

#include "logger.h"
#include "memalloc.h"
#include "tinymt32.h"

#include <string.h>

static void
init(uint32_t rng_seed);

static uint32_t *
select_dts(uint32_t num_dts, uint32_t stripe_width, uint32_t num_files, uint32_t fid);

static tinymt32_t rng;

file_dist fd_roundrobin =
    {FD_ROUNDROBIN, init, select_dts};

static void
init(uint32_t rng_seed)
{
  tinymt32_init(&rng, rng_seed);
}

static uint32_t *
select_dts(uint32_t num_dts, uint32_t stripe_width, uint32_t num_files, uint32_t fid)
{
  uint32_t *selected = malloc_assert(stripe_width * sizeof(uint32_t));

  uint32_t r = tinymt32_generate_uint32(&rng);
  int j = r % num_dts;
  for (int i = 0; i < stripe_width; i++)
  {
    selected[i] = j;
    j = (j + 1) % num_dts;
  }

  return selected;
}
