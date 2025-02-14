#include "exporter.h"

#include "csvexporter.h"
#include "logger.h"
#include "memalloc.h"
#include "timing.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static bool
is_dir (const char *out_path)
{
  struct stat stat_path;

  stat (out_path, &stat_path);
  return S_ISDIR (stat_path.st_mode);
}

char *
build_filepath (const param param)
{
  char *filepath;
  char *format = "%s/n%u_b%lu_s%d_m%u_w%u_z%lu_f%s-%f.csv";
  double timestamp = usectime ();
  int len = snprintf (filepath, 0, format, param.out_path, param.num_cli,
                      param.data_size, param.is_shared, param.num_dts,
                      param.stripe_width, param.stripe_size, param.file_dist,
                      timestamp);
  if (len > 0)
    {
      len++;
      filepath = malloc_assert (len);
      snprintf (filepath, len, format, param.out_path, param.num_cli,
                param.data_size, param.is_shared, param.num_dts,
                param.stripe_width, param.stripe_size, param.file_dist,
                timestamp);
    }

  return filepath;
}

void
export_results (const dts_results *results, const param param)
{
  if (is_dir (param.out_path))
    {
      char *filepath = build_filepath (param);
      export_csv (results, filepath);
    }
  else
    export_csv (results, param.out_path);
}