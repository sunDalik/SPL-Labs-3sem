#ifndef LAB5_MESSAGE_FORMAT_H
#define LAB5_MESSAGE_FORMAT_H

#include <stdint.h>

typedef enum {
    FIBONACCI,
    POW,
    BUBBLE_SORT_UINT64,
    STOP
} EType;

typedef struct {
    uint8_t Type;
    uint64_t Size;
    uint8_t *Data;
} TMessage;

#endif
