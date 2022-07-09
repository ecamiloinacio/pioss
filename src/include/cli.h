#ifndef CLI_H_
#define CLI_H_

#include <stdint.h>

void
cli_exec (uint32_t fid, uint64_t data_size, uint64_t stripe_size,
	  uint32_t stripe_width);

#endif
