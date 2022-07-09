#ifndef MDS_H_
#define MDS_H_

#include <stdint.h>

void
mds_init (const char *file_dist, uint32_t stripe_width, uint32_t num_files,
	  uint32_t num_dts, uint32_t rng_seed);
uint32_t*
mds_open (uint32_t fid);
void
mds_clean ();

#endif
