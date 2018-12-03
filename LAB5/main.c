#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rotation.h"
#include "bmp_io.h"

int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        puts("Arguments: [input file] [output file](optional)");
        return 1;
    }

    const char *input_path = argv[1];
    const char *output_path;
    if (argc == 2) {
        output_path = "out.bmp";
    } else {
        output_path = argv[2];
    }

    struct image *in_image = (struct image *) malloc(sizeof(struct image));
    switch (read_picture(input_path, in_image)) {
        case READ_OK: {
            puts("Image is loaded.");
            break;
        }
        case READ_FILENAME_NOT_FOUND: {
            puts("Input file name is not specified");
        }
        case READ_FILE_ERROR: {
            puts("Unable to open input file");
            return 1;
        }
        case READ_INVALID_BITS: {
            puts("Invalid input data");
            return 1;
        }
        case READ_INVALID_HEADER: {
            puts("Input file is not a bmp");
            return 1;
        }
        default: {
            puts("Undefined reading error");
            return 1;
        }
    }

    struct image *out_image = rotate(in_image);
    switch (write_picture(output_path, out_image)) {
        case WRITE_OK: {
            puts("Image is saved");
            return 0;
        }
        case WRITE_FILENAME_NOT_FOUND: {
            puts("Output file name is not specified");
            return 1;
        }
        case WRITE_IMAGE_NOT_FOUND: {
            puts("Output image is null.");
            return 1;
        }
        case WRITE_FILE_ERROR: {
            puts("Unable to open output file");
            return 1;
        }
        default: {
            puts("Undefined writing error");
            return 1;
        }
    }
}
