#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "file.h"
#include "logging.h"

struct LogLine *LogQueue[50];
int LogEntries = 0;

int disp_queue(time_t stamp, const char *file, int line, const char *function, int level, ...)
{
  char buf[128] = "";
  int err = errno;

  va_list ap;
  va_start(ap, level);
  const char *fmt = va_arg(ap, const char *);
  int ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (level == LL_PERROR)
  {
    ret += snprintf(buf + ret, sizeof(buf) - ret, ": %s", strerror(err));
    level = LL_ERROR;
  }

  struct LogLine *ll = calloc(1, sizeof(*ll));
  ll->time = time(NULL);
  ll->file = file;
  ll->line = line;
  ll->function = function;
  ll->level = level;
  ll->message = strdup(buf);

  LogQueue[LogEntries++] = ll;

  return ret;
}

void empty_queue(void)
{
  for (size_t i = 0; i < LogEntries; i++)
  {
    struct LogLine *ll = LogQueue[i];
    free(ll->message);
    free(ll);
  }

  memset(LogQueue, 0, sizeof(LogQueue));
  LogEntries = 0;
}

void flush_queue(dispatcher_t disp)
{
  for (size_t i = 0; i < LogEntries; i++)
  {
    struct LogLine *ll = LogQueue[i];
    disp(ll->time, ll->file, ll->line, ll->function, ll->level, "%s", ll->message);
    free(ll->message);
    free(ll);
  }

  memset(LogQueue, 0, sizeof(LogQueue));
  LogEntries = 0;
}

FILE *save_queue(void)
{
  char buf[32];
  strcpy(buf, "tmp-XXXXXX");
  int fd = mkstemp(buf);
  if (fd < 0)
    return NULL;

  FILE *fp = fdopen(fd, "a+");
  if (!fp)
    return NULL;

  unlink(buf);

  for (size_t i = 0; i < LogEntries; i++)
  {
    struct LogLine *ll = LogQueue[i];
    strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&ll->time));
    fprintf(fp, "[%s]<%c> %s", buf, levchars[ll->level + 2], ll->message);
    if (ll->level <= 0)
      fputs("\n", fp);
  }

  fflush(fp);
  rewind(fp);
  return fp;
}
