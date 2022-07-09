#include "timing.h"

#include "logger.h"
#include "memalloc.h"

#include <stddef.h>
#include <sys/time.h>
#include <time.h>

double
usectime ()
{
  double time;

  struct timeval timeval;
  gettimeofday (&timeval, NULL);
  time = timeval.tv_sec + (timeval.tv_usec / 1e6);

  return time;
}

char*
time_tostr (double usec)
{
  int str_len = 20;
  char *str = malloc_assert (str_len);

  time_t timer = (long int) usec;
  struct tm *tm = localtime (&timer);
  strftime (str, str_len, "%Y-%m-%d %H:%M:%S", tm);

  return str;
}
