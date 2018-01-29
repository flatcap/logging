#include <time.h>
#include <stdarg.h>
#include <stdio.h>

int disp_stdout(time_t stamp, const char *file, int line, const char *function, int level, ...)
{
  char buf[128] = "";

  va_list ap;
  va_start(ap, level);
  const char *fmt = va_arg(ap, const char *);
  int ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (level > 0)
    printf("%s", buf);
  else
    printf("%s\n", buf);

  return ret;
}

