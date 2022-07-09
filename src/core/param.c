#include "param.h"

#include "file_dist.h"
#include "logger.h"
#include "memalloc.h"
#include "timing.h"
#include "types.h"

#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const struct option long_opts[] =
  {
    { "num_cli", required_argument, NULL, 'n' },
    { "data_size", required_argument, NULL, 'b' },
    { "shared", no_argument, NULL, 's' },
    { "num_dts", required_argument, NULL, 'm' },
    { "stripe_width", required_argument, NULL, 'w' },
    { "stripe_size", required_argument, NULL, 'z' },
    { "file_dist", required_argument, NULL, 'f' },
    { "rng_seed", required_argument, NULL, 'r' },
    { "out_path", required_argument, NULL, 'o' },
    { "quiet", no_argument, NULL, 'q' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0 } };

static const char short_opts[] = "n:b:sm:w:z:f:r:o:qh";

static int
param_parse_uint32 (char *str, uint32_t *val)
{
  assert(str);

  char *endptr;

  *val = strtoul (str, &endptr, 10);

  if ((errno == ERANGE && *val == UINT_MAX) || (errno != 0 && *val == 0)
      || (endptr == str) || (*endptr != '\0'))
    {
      return errno;
    }

  return 0;
}

static int
param_parse_uint64 (char *str, uint64_t *val)
{
  assert(str);

  char *endptr;

  *val = strtoull (str, &endptr, 10);

  if ((errno == ERANGE && *val == UINT_MAX) || (errno != 0 && *val == 0)
      || (endptr == str) || (*endptr != '\0'))
    {
      return errno;
    }

  return 0;
}

static void
param_usage (char **argv)
{
  fprintf (stderr, "usage: %s [OPTIONS]\n\n", *argv);
  fputs (
      "OPTIONS:\n"
      "--num_cli=NUM | -n NUM      : number of simulated clients\n"
      "--data_size=NUM | -b NUM    : bytes transferred per simulated client\n"
      "--shared | -s               : on = single shared file; off = file per process\n"
      "--num_dts=NUM | -m NUM      : number of data servers\n"
      "--stripe_width=NUM | -w NUM : number of data servers per file\n"
      "--stripe_size=NUM | -z NUM  : size of a file chunk\n"
      "--file_dist=STR | -f STR    : file distribution algorithm\n"
      "--rng_seed=NUM | -r NUM     : seed for random number generation\n"
      "--out_path=STR | -o STR     : path for outputing simulation results\n"
      "--quiet | -q                : do not display parameters and results\n"
      "--help | -h                 : show this command-line options\n"
      "\n",
      stderr);
  fflush (stderr);
}

static void
param_default (param *param)
{
  param->num_cli = 1;
  param->data_size = MIB;
  param->is_shared = false;
  param->num_dts = 2;
  param->stripe_width = 1;
  param->stripe_size = 64 * KIB;
  strncpy (&(param->file_dist[0]), FD_RANDOM, strlen (FD_RANDOM));
  param->rng_seed = (uint32_t) (usectime () * 1e6);
  param->is_quiet = false;
}

int
param_parse (int argc, char **argv, param *param)
{
  assert(param);

  int c = 0;
  int optind = 0;
  bool error = 0;
  size_t len = 0;

  param_default (param);
  opterr = 0;

  while ((c = getopt_long (argc, argv, short_opts, long_opts, &optind)) != -1)
    {
      switch (c)
	{
	case 'n':
	  if (param_parse_uint32 (optarg, &(param->num_cli)))
	    {
	      logf (ERROR, "Invalid value for option --num_cli/-n: %s", optarg);
	      error = true;
	    }
	  break;
	case 'b':
	  if (param_parse_uint64 (optarg, &(param->data_size)))
	    {
	      logf (ERROR, "Invalid value for option --data_size/-b: %s",
		    optarg);
	      error = true;
	    }
	  break;
	case 's':
	  param->is_shared = true;
	  break;
	case 'm':
	  if (param_parse_uint32 (optarg, &(param->num_dts)))
	    {
	      logf (ERROR, "Invalid value for option --num_dts/-m: %s", optarg);
	      error = true;
	    }
	  break;
	case 'w':
	  if (param_parse_uint32 (optarg, &(param->stripe_width)))
	    {
	      logf (ERROR, "Invalid value for option --stripe_width/-w: %s",
		    optarg);
	      error = true;
	    }
	  break;
	case 'z':
	  if (param_parse_uint64 (optarg, &(param->stripe_size)))
	    {
	      logf (ERROR, "Invalid value for option --stripe_size/-z: %s",
		    optarg);
	      error = true;
	    }
	  break;
	case 'f':
	  if (strlen (optarg) > MAX_FILE_DIST_NAME)
	    {
	      logf (ERROR, "Invalid value for option --file_dist/-f: %s",
		    optarg);
	      error = true;
	    }
	  else
	    strncpy (&(param->file_dist[0]), optarg, MAX_FILE_DIST_NAME);
	  break;
	case 'r':
	  if (param_parse_uint32 (optarg, &(param->rng_seed)))
	    {
	      logf (ERROR, "Invalid value for option --rng_seed/-r: %s",
		    optarg);
	      error = true;
	    }
	  break;
	case 'o':
	  if (strlen (optarg) > PATH_MAX)
	    {
	      logf (ERROR, "Invalid value for option --out_path/-o: %s",
		    optarg);
	      error = true;
	    }
	  else
	    strncpy (&(param->out_path[0]), optarg, PATH_MAX);
	  break;
	case 'q':
	  param->is_quiet = true;
	  break;
	case 'h':
	  param_usage (argv);
	  exit (EXIT_SUCCESS);
	case '?':
	default:
	  logf (ERROR, "Invalid option: %c", optopt);
	  error = true;
	}
    }

  if (optind > 0 && optind < argc)
    {
      error = true;
      while (optind < argc)
	logf (ERROR, "Invalid argument: %s", argv[optind++]);
    }

  if (error)
    param_usage (argv);

  return error;
}

int
param_validate (const param param)
{
  int error = 0;

  if (param.num_cli == 0)
    {
      log (ERROR, "'num_cli' must be greater than zero");
      error = 1;
    }

  if (param.data_size == 0)
    {
      log (ERROR, "'data_size' must be greater than zero");
      error = 1;
    }

  if (param.num_dts == 0)
    {
      log (ERROR, "'num_dts' must be greater than zero");
      error = 1;
    }

  if (param.stripe_width == 0)
    {
      log (ERROR, "'stripe_width' must be greater than zero");
      error = 1;
    }
  else if (param.stripe_width > param.num_dts)
    {
      log (ERROR, "'stripe_width' cannot be greater than 'num_dts'");
      error = 1;
    }

  if (strcmp (FD_RANDOM, param.file_dist) != 0
      && strcmp (FD_ROUNDROBIN, param.file_dist) != 0
      && strcmp (FD_N2R2, param.file_dist) != 0)
    {
      logf (ERROR, "'file_dist' must be one of the following: %s, %s, %s",
	    (FD_RANDOM), (FD_ROUNDROBIN), (FD_N2R2));
      error = 1;
    }

  return error;
}

char*
param_tostr (const param param)
{
  const char *format =
      "** Parameters **********************************************\n"
	  "\n"
	  "      num_cli = %u\n"
	  "    data_size = %lu\n"
	  "    is_shared = %s\n"
	  "      num_dts = %u\n"
	  " stripe_width = %u\n"
	  "  stripe_size = %lu\n"
	  "    file_dist = %s\n"
	  "     rng_seed = %u\n"
	  "     out_path = %s\n"
	  "\n"
	  "************************************************************\n\n";

  char *content;
  int len = snprintf (content, 0, format, param.num_cli, param.data_size,
		      (param.is_shared ? "true" : "false"), param.num_dts,
		      param.stripe_width, param.stripe_size, param.file_dist,
		      param.rng_seed, param.out_path);
  if (len > 0)
    {
      len++;
      content = malloc_assert (len);
      snprintf (content, len, format, param.num_cli, param.data_size,
		(param.is_shared ? "true" : "false"), param.num_dts,
		param.stripe_width, param.stripe_size, param.file_dist,
		param.rng_seed, param.out_path);
    }

  return content;
}
