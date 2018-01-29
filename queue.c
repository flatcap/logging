#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logging.h"

struct LogLine *LogQueue[10];
int LogEntries = 0;

int disp_queue(time_t stamp, const char *file, int line, const char *function, int level, ...)
{
  char buf[128] = "";

  va_list ap;
  va_start(ap, level);
  const char *fmt = va_arg(ap, const char *);
  int ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

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
