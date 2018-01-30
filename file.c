#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "logging.h"

const char *log_file = "log.txt";
const char *levchars = "PEWM12345";

int disp_file(time_t stamp, const char *file, int line, const char *function, int level, ...)
{
  static char buf[23] = "";
  static time_t last = 0;
  int ret = 0;
  int err = errno;

  FILE *fp = fopen(log_file, "a");
  if (!fp)
    return 0;

  time_t now = stamp ? stamp : time(NULL);
  if (now > last)
  {
    ret += strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    last = now;
  }

  ret += fprintf(fp, "[%s]<%c> %s() ", buf, levchars[level + 3], function);

  va_list ap;
  va_start(ap, level);
  const char *fmt = va_arg(ap, const char *);
  ret = vfprintf(fp, fmt, ap);
  va_end(ap);

  if (level == LL_PERROR)
  {
    fprintf(fp, ": %s\n", strerror(err));
  }
  else if (level <= 0)
  {
    fputs("\n", fp);
    ret++;
  }

  fclose(fp);
  return ret;
}
