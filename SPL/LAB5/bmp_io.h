#ifndef LAB5_BMP_IO_H
#define LAB5_BMP_IO_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "bmp_struct.h"

enum read_error_code {
    READ_OK = 0,
    READ_FILENAME_NOT_FOUND,
    READ_FILE_ERROR,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
} ;

enum write_error_code {
    WRITE_OK = 0,
    WRITE_FILE_ERROR,
    WRITE_IMAGE_NOT_FOUND,
    WRITE_FILENAME_NOT_FOUND,
} ;


enum read_error_code read_picture(char const* filename, struct image* in_bmp);
enum write_error_code write_picture(char const* filename, struct image const* image);

#endif
