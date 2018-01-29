#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

const char *log_file = "log.txt";

typedef int (*dispatcher_t)(time_t stamp, const char *file, int line,
                            const char *function, int level, ...);

dispatcher_t logger = NULL;

enum LogLevel
{
  LL_ERROR = -2,
  LL_WARNING = -1,
  LL_MESSAGE = 0,
  LL_DEBUG1 = 1,
  LL_DEBUG2 = 2,
  LL_DEBUG3 = 3,
  LL_DEBUG4 = 4,
  LL_DEBUG5 = 5,
};

struct LogLine
{
  time_t time;
  const char *file;
  int line;
  const char *function;
  int level;
  char *message;
};

struct LogLine *LogQueue[10];
int LogEntries = 0;

// clang-format off
#define debug(LEVEL, ...) logger(0, __FILE__, __LINE__, __func__, LEVEL,      __VA_ARGS__)
#define message(...)      logger(0, __FILE__, __LINE__, __func__, LL_MESSAGE, __VA_ARGS__)
#define warning(...)      logger(0, __FILE__, __LINE__, __func__, LL_WARNING, __VA_ARGS__)
#define error(...)        logger(0, __FILE__, __LINE__, __func__, LL_ERROR,   __VA_ARGS__)
// clang-format on

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

void print_time()
{
  char buf[32];
  time_t now = time(NULL);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
  printf("%s\n", buf);
}

int main()
{
  // logger = disp_stdout;
  // logger = disp_curses;
  // logger = disp_file;
  logger = disp_queue;

  // logger(123, __FILE__, __LINE__, __func__, 2, "direct %s to logger\n", "call");

  print_time();
  error("danger %s!", "Will Robinson");
  warning("warning %d %s", 42, "don't panic");
  message("message %2.2f", 3.141592654);
  debug(3, "hello %s\n", "world");

  sleep(5);
  flush_queue(disp_curses);
  // flush_queue(disp_file);
  print_time();

  return 0;
}
