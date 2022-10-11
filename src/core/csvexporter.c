#include "csvexporter.h"

#include "logger.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static FILE *
create_file (const char *out_path)
{
  FILE *file = NULL;

  if (access (out_path, F_OK) != -1)
    logf (ERROR, "Informed output file already exists: %s", out_path);
  else
    {
      file = fopen (out_path, "w+");
      if (!file)
        {
          int errnum = errno;
          logf (ERROR, "Failed to create output file: %s (%d: %s)", out_path,
                errnum, strerror (errnum));
        }
    }

  return file;
}

void
export_csv (const dts_results *results, const char *out_path)
{
  FILE *file = create_file (out_path);

  if (file)
    {
      fputs ("DTS,Bytes\n", file);
      uint32_t num_dts = results->num_dts;
      for (int i = 0; i < num_dts; i++)
        fprintf (file, "%u,%lu\n", i, results->bytes_stored[i]);

      fflush (file);
      fclose (file);
    }
}
