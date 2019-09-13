#include <stdio.h>
#include "debug.h"
#include "mem_alloc.h"

int main() {

    struct mem *heap_start = (struct mem *) heap_space_init();
    puts("\tempty heap:");
    memalloc_debug_heap(stdout, heap_start);
    puts("");

    char *p1 = custom_alloc(4096);
    for (int i = 0; i < 4096; ++i) {
        p1[i] = (char) i;
    }
    printf("\tp1 = %s = custom_alloc(4096):\n", p1);
    memalloc_debug_heap(stdout, heap_start);
    puts("");

    char *p2 = custom_alloc(42);
    p2[3] = 'a';
    printf("\tp2 = %s = custom_alloc(42):\n", p2);
    memalloc_debug_heap(stdout, heap_start);
    puts("");

    puts("\tcustom_free(p1):");
    custom_free(p1);
    memalloc_debug_heap(stdout, heap_start);
    puts("");

    puts("\tcustom_free(p2):");
    custom_free(p2);
    memalloc_debug_heap(stdout, heap_start);
    puts("");

    char *p3 = custom_alloc(2);
    printf("\tp3 = %s = custom_alloc(2):\n", p3);
    memalloc_debug_heap(stdout, heap_start);
    puts("");

    return 0;
}