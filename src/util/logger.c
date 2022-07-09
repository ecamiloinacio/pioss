#include "logger.h"

#include "memalloc.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void
log (enum TAG_ENUM tag, const char *message)
{
  assert(message);

  time_t now;
  time (&now);

  FILE *stream;
  switch (tag)
    {
    case ERROR:
      stream = stderr;
      break;
    case INFO:
    case DEBUG:
    default:
      stream = stdout;
      break;
    }

  fprintf (stream, "%s [%s]: %s\n", strtok (ctime (&now), "\n"),
	   TAG_STRING[tag], message);
}

void
logf (enum TAG_ENUM tag, const char *format, ...)
{
  assert(format);

  va_list ap;
  va_list copy;
  char *message;
  int len;

  va_start(ap, format);
  va_copy(copy, ap);

  len = vsnprintf (message, 0, format, copy);
  if (len > 0)
    {
      len++;
      message = malloc_assert (len);
      vsnprintf (message, len, format, ap);
    }

  va_end(copy);
  va_end(ap);

  log (tag, message);
}
