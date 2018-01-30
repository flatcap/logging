#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "curses.h"
#include "file.h"
#include "logging.h"
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

void sample_calls(void)
{
  errno = EPERM;
  perror("function");
  error("danger %s!", "Will Robinson");
  warning("warning %d %s", 42, "don't panic");
  message("message %2.2f", 3.141592654);
  debug(3, "hello %s\n", "world");
}

void test_stdout(void)
{
  logger = disp_stdout;
  sample_calls();
}

void test_curses(void)
{
  logger = disp_curses;
  sample_calls();
}

void test_file(void)
{
  logger = disp_file;
  sample_calls();
}

void test_queue(void)
{
  logger = disp_queue;
  print_time();
  sample_calls();
  sleep(3);
  flush_queue(disp_curses);
  print_time();
}

void test_memory(void)
{
  char line[128];
  FILE *fp = NULL;

  logger = disp_queue;

  for (size_t i = 0; i < 5; i++)
    warning("before %ld", i);

  fp = save_queue();
  printf("------------------------------\n");
  while (fgets(line, sizeof(line), fp))
    printf("%s", line);
  fclose(fp);

  for (size_t i = 0; i < 5; i++)
    warning("after %ld", i);

  fp = save_queue();
  printf("------------------------------\n");
  while (fgets(line, sizeof(line), fp))
    printf("%s", line);
  fclose(fp);

  printf("------------------------------\n");
  empty_queue();
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    return 1;

  // logger(123, __FILE__, __LINE__, __func__, 2, "direct %s to logger\n", "call");

  switch (argv[1][0])
  {
    case 'c':
      test_curses();
      break;
    case 'f':
      test_file();
      break;
    case 'm':
      test_memory();
      break;
    case 'q':
      test_queue();
      break;
    case 's':
      test_stdout();
      break;
    default:
      return 1;
  }

  return 0;
}
