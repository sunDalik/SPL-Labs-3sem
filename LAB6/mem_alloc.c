#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "mem_alloc.h"

char *heap_space_start;

void *heap_space_init() {
    heap_space_start = mmap(HEAP_START, MINIMAL_HEAP_SIZE, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);
    ((struct mem *) heap_space_start)->next = NULL;
    ((struct mem *) heap_space_start)->capacity = MINIMAL_HEAP_SIZE - sizeof(struct mem);
    ((struct mem *) heap_space_start)->is_free = true;
    return (void *) heap_space_start;
}

void merge_free_chunks(struct mem *const chunk) {
    while (chunk->next != NULL && chunk->next->is_free) {
        chunk->capacity = chunk->capacity + chunk->next->capacity + sizeof(struct mem);
        chunk->next = chunk->next->next;
    }
}

void custom_free(void *memChunk) {
    struct mem *chunk = (struct mem *) ((char *) memChunk - sizeof(struct mem));
    chunk->is_free = true;
    merge_free_chunks(chunk);
}

void *custom_alloc(size_t query) {
    if (query % CHUNK_ALIGN != 0) query += CHUNK_ALIGN - (query % CHUNK_ALIGN); //aligning size

    struct mem *chunk = (struct mem *) heap_space_start;
    while (true) {
        //merging free chunks
        if (chunk->is_free) {
            merge_free_chunks(chunk);
            if (chunk->capacity >= query) break;  //stop building block if it is enough
        }

        //adding new free chunk
        if (chunk->next == NULL) {
            void *heap_end = (char *) chunk + sizeof(struct mem) + chunk->capacity;
            void *new_area = mmap(heap_end, MINIMAL_HEAP_SIZE, PROT_READ | PROT_WRITE,
                                  MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);

            if (new_area != MAP_FAILED && chunk->is_free) {
                chunk->capacity = chunk->capacity + MINIMAL_HEAP_SIZE;
                break;
            } else if (new_area == MAP_FAILED) {
                new_area = mmap(NULL, MINIMAL_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
            } else {
                struct mem *new_chunk_head = (struct mem *) new_area;
                new_chunk_head->next = NULL;
                new_chunk_head->capacity = MINIMAL_HEAP_SIZE - sizeof(struct mem);
                new_chunk_head->is_free = true;
                chunk->next = new_chunk_head;
                chunk = chunk->next;
            }
        } else chunk = chunk->next;
    }
    chunk->is_free = false;

    //handling free space after allocation
    size_t remaining_capacity = chunk->capacity - query;
    if (remaining_capacity < CHUNK_MIN_SIZE) {
        //skip header and return pointer
        return (void *) (chunk + 1);
    }

    chunk->capacity = query;
    struct mem *new_chunk = (struct mem*)((char *) chunk + sizeof(struct mem) + query);
    new_chunk->next = chunk->next;
    new_chunk->capacity = remaining_capacity - sizeof(struct mem);
    new_chunk->is_free = true;
    chunk->next = new_chunk;
    return (void *) (chunk + 1);
}