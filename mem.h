#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

typedef struct memory {
  void *data;
  unsigned int size;
  unsigned int pos;
  unsigned int free;
} memory;

memory *memory_new(unsigned int max_size);

/* Returns a pointer to the allocated data */
void *memory_allocate(memory* mem, unsigned int size);

memory memory_init(void *data, unsigned int size);

void memory_free(memory *mem, unsigned int size);

void memory_clear(memory *mem);

#endif
