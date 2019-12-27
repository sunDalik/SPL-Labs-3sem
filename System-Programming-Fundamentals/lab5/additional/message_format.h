#pragma once

#include <stdint.h>

typedef enum {
    FIBONACCI = 0,
    POW = 1,
    BUBBLE_SORT_UINT64 = 2,
    STOP = 3
} EType;

typedef struct __attribute__((__packed__)) {
    uint8_t Type;
    uint64_t Size;
    uint8_t *Data;
} TMessage;