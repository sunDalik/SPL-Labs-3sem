#pragma once

#include <stdint.h>

typedef enum {
    FIBONACCI = 0,
    POW = 1,
    BUBBLE_SORT_UINT64 = 2,
    NONE = 99
} EType;

typedef struct __attribute__((__packed__)) {
    uint8_t Type;
    uint32_t Size;
    uint32_t *Data;
} TMessage;