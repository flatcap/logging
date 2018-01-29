#ifndef _QUEUE_H
#define _QUEUE_H

#include <time.h>
#include "logging.h"

int disp_queue(time_t stamp, const char *file, int line, const char *function, int level, ...);
void flush_queue(dispatcher_t disp);

#endif /* _QUEUE_H */
