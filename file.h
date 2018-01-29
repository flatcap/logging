#ifndef _FILE_H
#define _FILE_H

#include <time.h>

extern const char *levchars;

int disp_file(time_t stamp, const char *file, int line, const char *function, int level, ...);

#endif /* _FILE_H */
