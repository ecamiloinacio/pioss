#include "cli.h"

#include "dts.h"
#include "logger.h"
#include "mds.h"
#include "memalloc.h"

#include <stdlib.h>

void
cli_exec (uint32_t fid, uint64_t data_size, uint64_t stripe_size,
	  uint32_t stripe_width)
{
  uint32_t *dts_list = mds_open (fid);
  uint64_t *dts_buffer = calloc_assert (stripe_width, sizeof(uint64_t));
  uint32_t curdts = 0;

  uint64_t num_chunks = data_size / stripe_size;

  uint64_t num_chunks_per_dts = num_chunks / stripe_width;
  if (num_chunks_per_dts > 0)
    for (curdts = 0; curdts < stripe_width; curdts++)
      dts_buffer[curdts] += num_chunks_per_dts * stripe_size;

  curdts = 0;
  uint64_t remaining_chunks = num_chunks % stripe_width;
  if (remaining_chunks > 0)
    for (int i = 0; i < remaining_chunks; i++)
      {
	dts_buffer[curdts] += stripe_size;
	curdts = (curdts + 1) % stripe_width;
      }

  uint64_t remaining_bytes = data_size % stripe_size;
  if (remaining_bytes > 0)
    dts_buffer[curdts] += remaining_bytes;

  for (int i = 0; i < stripe_width; i++)
    dts_store (dts_list[i], dts_buffer[i]);

  free (dts_buffer);
}
