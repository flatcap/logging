#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "logging.h"
#include "curses.h"
#include "file.h"
#include "queue.h"
#include "stdout.h"

dispatcher_t logger = NULL;

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
