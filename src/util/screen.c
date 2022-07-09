#include "screen.h"

#include "memalloc.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

void
screen (char *content)
{
  assert(content);

  fprintf (stdout, "%s\n", content);
}

void
screenf (char *format, ...)
{
  assert(format);

  va_list ap;
  va_list copy;
  char *content;
  int len;

  va_start(ap, format);
  va_copy(copy, ap);

  len = vsnprintf (content, 0, format, copy);
  if (len > 0)
    {
      len++;
      content = malloc_assert (len);
      vsnprintf (content, len, format, ap);
    }

  va_end(copy);
  va_end(ap);

  screen (content);
}
