#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>

#ifndef LAB6_MEM_ALLOC
#define LAB6_MEM_ALLOC

#define HEAP_START ((void*)0x04040000)
#define MINIMAL_HEAP_SIZE (sysconf(_SC_PAGESIZE))
#define CHUNK_ALIGN 8
#define CHUNK_MIN_SIZE (sizeof(struct mem) + CHUNK_ALIGN)

struct mem {
    struct mem* next;
    size_t capacity;
    bool is_free;
};

void* custom_alloc(size_t query);

void custom_free(void *memChunk);

void* heap_space_init();

#endif
