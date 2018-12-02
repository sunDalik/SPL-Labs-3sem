//
// Created by dell on 06.11.18.
//

#ifndef SPLLAB5_IN_OUT_BMP_H
#define SPLLAB5_IN_OUT_BMP_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "bmp_pic_struct.h"

enum read_error_code {
    READ_OK = 0,
    READ_FILE_NOT_FOUND,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
} ;

enum write_error_code {
    WRITE_OK = 0,
    WRITE_ERROR,
    WRITE_IMAGE_NOT_FOUND,
    WRITE_FILENAME_NOT_FOUND,
} ;


enum read_error_code read_picture(char const* filename, struct image* new_image);
enum write_error_code write_picture(char const* filename, struct image const* image);

#endif //SPLLAB5_IN_OUT_BMP_H
