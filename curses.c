#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include "file.h"

int disp_curses(time_t stamp, const char *file, int line, const char *function, int level, ...)
{
  char buf[128] = "";

  va_list ap;
  va_start(ap, level);
  const char *fmt = va_arg(ap, const char *);
  int ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  int colour = 0;
  switch (level)
  {
    case -2:
      colour = 31;
      break;
    case -1:
      colour = 33;
      break;
    case 0:
      colour = 36;
      break;
  }

  disp_file(stamp, file, line, function, level, "%s", buf);

  if (level > 0)
    printf("%s", buf);
  else
    printf("\033[1;%dm%s\033[0m\n", colour, buf);

  return ret;
}
