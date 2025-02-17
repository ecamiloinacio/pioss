#ifndef FILE_DIST_H_
#define FILE_DIST_H_

#include <stdint.h>

#define FD_RANDOM "Random"
#define FD_ROUNDROBIN "RoundRobin"
#define FD_N2R2 "N2R2"

typedef struct file_dist
{
  char *fd_name;
  void (*init)(uint32_t rnd_seed);
  uint32_t *(*select_dts)(uint32_t num_dts, uint32_t stripe_width, uint32_t num_files, uint32_t fid);
} file_dist;

extern file_dist fd_random;
extern file_dist fd_roundrobin;
extern file_dist fd_n2r2;

#endif
