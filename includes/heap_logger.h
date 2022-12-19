#ifndef __HEAP_LOGGER_H__
#define __HEAP_LOGGER_H__

#include <stddef.h>

void clean_log(void);
void write_log(char *action, size_t size, void *ptr);
void read_log(void);

#endif /* __HEAP_LOGGER_H__ */
