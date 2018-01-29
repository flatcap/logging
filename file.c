#include <stdarg.h>
#include <stdio.h>
#include <time.h>

const char *log_file = "log.txt";

int disp_file(time_t stamp, const char *file, int line, const char *function, int level, ...)
{
  const char *levchars = "EWM12345";
  static char buf[23] = "";
  static time_t last = 0;
  int ret = 0;

  FILE *fp = fopen(log_file, "a");
  if (!fp)
    return 0;

  time_t now = stamp ? stamp : time(NULL);
  if (now > last)
  {
    ret += strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    last = now;
  }

  ret += fprintf(fp, "[%s]<%c> %s() ", buf, levchars[level + 2], function);

  va_list ap;
  va_start(ap, level);
  const char *fmt = va_arg(ap, const char *);
  ret = vfprintf(fp, fmt, ap);
  if (level <= 0)
  {
    fputs("\n", fp);
    ret++;
  }
  va_end(ap);

  fclose(fp);
  return ret;
}
