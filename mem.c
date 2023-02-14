#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct memory {
  void *data;
  unsigned int size;
  unsigned int pos;
  unsigned int free;
} memory;

memory *memory_new(unsigned int max_size) {
  memory *m = malloc(sizeof(memory));
  m->data   = malloc(max_size);
  m->size   = max_size;
  m->pos    = 0;
  m->free   = max_size;

  memset(m->data, 0, max_size);

  return m;
}

/* Returns a pointer to the allocated data */
void *memory_allocate(memory* mem, unsigned int size) {
  void* data = NULL;

  if (mem->pos + size <= mem->size) {
    data = (void*)((unsigned long)mem->data + mem->pos);
    mem->pos  += size;
    mem->free -= size;
  } else {
    //perror("Trying to allocate %lu in a %lu sized memory block", size, mem->size);
    perror("No more room!");
    exit(EXIT_FAILURE);
  }

  return data;
}

memory memory_init(void *data, unsigned int size) {
  memory m = {0};
  m.data = data;
  m.size = size;
  m.free = 0;
  return m;
}

void memory_free(memory *mem, unsigned int size) {
  if (size > mem->pos) {
    perror("Freeing too much memory!");
    exit(EXIT_FAILURE);
  } else {
    mem->pos  -= size;
    mem->free += size;
  }
}

void memory_clear(memory *mem) {
	mem->pos  = 0;
	mem->free = mem->size;
}
