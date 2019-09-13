#include <stdio.h>
#include "mem_alloc.h"

#ifndef LAB6_DEBUG
#define LAB6_DEBUG

#define DEBUG_FIRST_BYTES 8

void memalloc_debug_struct_info( FILE* f, struct mem const* const address );

void memalloc_debug_heap( FILE* f, struct mem const* ptr );

#endif
