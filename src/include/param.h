#ifndef PARAM_H_
#define PARAM_H_

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_FILE_DIST_NAME 16
#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

typedef struct param
{
  uint32_t num_cli;
  uint64_t data_size;
  bool is_shared;
  uint32_t num_dts;
  uint32_t stripe_width;
  uint64_t stripe_size;
  char file_dist[MAX_FILE_DIST_NAME];
  uint32_t rng_seed;
  char out_path[PATH_MAX];
  bool is_quiet;
} param;

int
param_parse (int argc, char **argv, param *param);
int
param_validate (const param param);
char *
param_tostr (const param param);

#endif
