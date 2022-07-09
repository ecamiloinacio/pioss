#include "logger.h"
#include "param.h"
#include "pioss.h"
#include "screen.h"

#include <stdlib.h>

int
main (int argc, char **argv)
{
  screen ("PIOSS - The Parallel I/O and Storage System simulation model");
  screen ("                    -= Lite Version =-                      \n");

  param param =
    { };
  if (param_parse (argc, argv, &param))
    {
      log (ERROR, "Error while parsing parameters; aborting.");
      exit (EXIT_FAILURE);
    }

  if (param_validate (param))
    {
      log (ERROR, "Invalid parameters encountered; aborting.");
      exit (EXIT_FAILURE);
    }

  if (!param.is_quiet)
    screen (param_tostr (param));

  pioss_exec (param);

  return EXIT_SUCCESS;
}
