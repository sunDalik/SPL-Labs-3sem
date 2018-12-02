#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "rotation.h"

struct image* rotate(struct image const* rotatable){

    int row;
    int col;

    struct image* rotated = (struct image*)malloc(sizeof(struct image));
    rotated->width = rotatable->height;
    rotated->height = rotatable->width;
    rotated->data = (struct pixel*)malloc(rotatable->height*rotatable->width* sizeof(struct pixel));

    for (row = 0; row < rotatable->height; row++){
        for (col = 0; col < rotatable->width; col++) {
            rotated->data[((rotatable->width - 1 - col) * rotatable->height) + row] = rotatable->data[row*rotatable->width + col];
        }
    }
    return rotated;
}

