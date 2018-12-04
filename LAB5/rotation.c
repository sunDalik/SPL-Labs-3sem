#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "rotation.h"

struct image *rotate(struct image const *picture) {
    struct image *rotated_picture = (struct image *) malloc(sizeof(struct image));
    rotated_picture->width = picture->height;
    rotated_picture->height = picture->width;
    rotated_picture->data = (struct pixel *) malloc(picture->height * picture->width * sizeof(struct pixel));

    for (int row = 0; row < picture->height; row++) {
        for (int col = 0; col < picture->width; col++) {
            rotated_picture->data[((rotated_picture->height - col - 1) * rotated_picture->width) + row] = picture->data[
                    row * picture->width + col];
        }
    }
    return rotated_picture;
}


