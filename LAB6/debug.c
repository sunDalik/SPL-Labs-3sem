#include "debug.h"
#include "mem_alloc.h"
#include <stdlib.h>

void memalloc_debug_struct_info(FILE *f, struct mem const *const address) {
    size_t i;
    fprintf(f,
            "[%p], size: %lu, is_free: %d\n",
            (void *) address,
            address->capacity,
            address->is_free);
    for (i = 0; i < DEBUG_FIRST_BYTES && i < address->capacity; ++i) {
        fprintf(f, "%02hhX ", ((char *) address)[sizeof(struct mem) + i]); 
    }
    puts("");
}

void memalloc_debug_heap(FILE *f, struct mem const *ptr) {
    for (; ptr; ptr = ptr->next)
        memalloc_debug_struct_info(f, ptr);
}