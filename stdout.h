#ifndef _STDOUT_H
#define _STDOUT_H

#include <time.h>

int disp_stdout(time_t stamp, const char *file, int line, const char *function, int level, ...);

#endif /* _STDOUT_H */
