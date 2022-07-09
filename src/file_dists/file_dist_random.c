#include "file_dist.h"

#include "logger.h"
#include "memalloc.h"
#include "tinymt32.h"

#include <string.h>

static void
init (uint32_t rng_seed);

static uint32_t*
select_dts (uint32_t num_dts, uint32_t stripe_width);

static tinymt32_t rng;

file_dist fd_random =
  { FD_RANDOM, init, select_dts };

static void
init (uint32_t rng_seed)
{
  tinymt32_init (&rng, rng_seed);
}

static uint32_t*
select_dts (uint32_t num_dts, uint32_t stripe_width)
{
  size_t selected_len = stripe_width * sizeof(uint32_t);
  uint32_t *selected = malloc_assert (selected_len);
  uint32_t *dts = malloc_assert (num_dts * sizeof(uint32_t));

  uint32_t r;
  int i, j, k;
  for (i = 0; i < num_dts; i++)
    dts[i] = i;

  for (i = 0; i < num_dts; i++)
    {
      r = tinymt32_generate_uint32 (&rng);
      j = r % num_dts;
      k = dts[i];
      dts[i] = dts[j];
      dts[j] = k;
    }

  memcpy (selected, dts, selected_len);

  return selected;
}
