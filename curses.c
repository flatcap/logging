#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "file.h"
#include "logging.h"

int disp_curses(time_t stamp, const char *file, int line, const char *function, int level, ...)
{
  char buf[128] = "";
  int err = errno;

  va_list ap;
  va_start(ap, level);
  const char *fmt = va_arg(ap, const char *);
  int ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  int colour = 0;
  switch (level)
  {
    case LL_PERROR:
    case LL_ERROR:
      colour = 31;
      break;
    case LL_WARNING:
      colour = 33;
      break;
    case LL_MESSAGE:
      colour = 36;
      break;
  }

  disp_file(stamp, file, line, function, level, "%s", buf);

  if (level > LL_MESSAGE)
    printf("%s", buf);
  else if (level == LL_PERROR)
    printf("\033[1;%dm%s: %s\033[0m\n", colour, buf, strerror(err));
  else
    printf("\033[1;%dm%s\033[0m\n", colour, buf);

  return ret;
}
