#ifndef _LOGGING_H
#define _LOGGING_H

#include <time.h>

typedef int (*dispatcher_t)(time_t stamp, const char *file, int line, const char *function, int level, ...);

enum LogLevel
{
  LL_PERROR  = -3,
  LL_ERROR   = -2,
  LL_WARNING = -1,
  LL_MESSAGE = 0,
  LL_DEBUG1  = 1,
  LL_DEBUG2  = 2,
  LL_DEBUG3  = 3,
  LL_DEBUG4  = 4,
  LL_DEBUG5  = 5,
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

#define debug(LEVEL, ...) logger(0, __FILE__, __LINE__, __func__, LEVEL,      __VA_ARGS__)
#define message(...)      logger(0, __FILE__, __LINE__, __func__, LL_MESSAGE, __VA_ARGS__)
#define warning(...)      logger(0, __FILE__, __LINE__, __func__, LL_WARNING, __VA_ARGS__)
#define error(...)        logger(0, __FILE__, __LINE__, __func__, LL_ERROR,   __VA_ARGS__)
#define perror(...)       logger(0, __FILE__, __LINE__, __func__, LL_PERROR,  __VA_ARGS__)

#endif /* _LOGGING_H */
