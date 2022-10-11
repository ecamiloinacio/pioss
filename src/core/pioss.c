#include "pioss.h"

#include "cli.h"
#include "exporter.h"
#include "dts.h"
#include "logger.h"
#include "mds.h"
#include "screen.h"
#include "timing.h"

#include <stdlib.h>
#include <string.h>

static void
pioss_init (const param param)
{
  dts_init (param.num_dts);

  uint32_t num_files = param.is_shared ? 1 : param.num_cli;
  mds_init (param.file_dist, param.stripe_width, num_files, param.num_dts,
            param.rng_seed);
}

static void
pioss_clean ()
{
  dts_clean ();
  mds_clean ();
}

static void
pioss_show_dts_results (dts_results *results)
{
  screenf ("%7s %24s", "DTS", "Bytes");
  uint32_t num_dts = results->num_dts;
  for (int i = 0; i < num_dts; i++)
    screenf ("%7u %24lu", i, results->bytes_stored[i]);
  screen ("");
}

void
pioss_exec (const param param)
{
  pioss_init (param);

  double start_time = usectime ();
  screenf ("Simulation started at %s\n", time_tostr (start_time));

  for (int i = 0; i < param.num_cli; i++)
    {
      uint32_t fid = param.is_shared ? 0 : i;
      cli_exec (fid, param.data_size, param.stripe_size, param.stripe_width);
    }

  double end_time = usectime ();
  screenf ("Simulation ended at %s", time_tostr (end_time));
  screenf ("Duration: %.6f seconds\n", end_time - start_time);

  dts_results *results = dts_get_results ();
  if (!param.is_quiet)
    {
      screen ("Results:\n");
      pioss_show_dts_results (results);
    }

  if (strlen (param.out_path) > 0)
    export_results (results, param);

  free (results);

  pioss_clean ();
}
