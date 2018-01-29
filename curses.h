#ifndef _CURSES_H
#define _CURSES_H

#include <time.h>

int disp_curses(time_t stamp, const char *file, int line, const char *function, int level, ...);
  
#endif /* _CURSES_H */
