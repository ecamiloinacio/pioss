#include "file_dist.h"

#include "logger.h"
#include "memalloc.h"
#include "math_util.h"
#include "tinymt32.h"

#include <stdbool.h>
#include <string.h>

static void
init(uint32_t rng_seed);

static uint32_t *
select_dts(uint32_t num_dts, uint32_t stripe_width, uint32_t num_files, uint32_t fid);

static tinymt32_t rng;

static uint32_t baseline_dts;
static bool baseline_set;

file_dist fd_n2r2 =
    {FD_N2R2, init, select_dts};

static void
init(uint32_t rng_seed)
{
  tinymt32_init(&rng, rng_seed);

  baseline_set = false;
}

static uint32_t *
select_dts(uint32_t num_dts, uint32_t stripe_width, uint32_t num_files, uint32_t fid)
{
  uint32_t *selected = malloc_assert(stripe_width * sizeof(uint32_t));

  if (!baseline_set)
  {
    baseline_dts = tinymt32_generate_uint32(&rng) % num_dts;
    baseline_set = true;
  }

  uint32_t g = max((num_dts / num_files), 1);
  uint32_t next_dts = (baseline_dts + (fid * g)) % num_dts;
  for (int i = 0; i < stripe_width; i++)
  {
    selected[i] = next_dts;
    next_dts = (next_dts + 1) % num_dts;
  }

  return selected;
}
