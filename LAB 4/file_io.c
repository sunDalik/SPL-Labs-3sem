#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

typedef int bool;
#define true 1
#define false 0


bool load(struct list** list, const char* filename){
    int e;
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        return false;
    }
    while ( fscanf(input_file, "%d", &e) != EOF) {
        list_add_back(list, e);
    }
    fclose(input_file);
    return true;
}


bool save(struct list* list, const char* filename){
    FILE* output_file = fopen(filename, "w+");
    if (output_file == NULL){
        return false;
    }
    while (list != NULL) {
        fprintf(output_file, "%d ", list->element);
        list = list->succ;
    }
    fclose(output_file);
    return true;
}


bool serialize(struct list* list, const char* filename){
	size_t i = 0, list_len;
	int* buf;
    FILE* output_file = fopen(filename, "wb");
    if (output_file == NULL) {
        return false;
    }
    list_len = list_length(list);
    buf = (int*) malloc(list_len * sizeof(int));
    for (i = 0; i < list_len; i++) {
        buf[i] = list->element;
        list = list->succ;
    }
    fwrite(buf, list_len, sizeof(int), output_file);
    fclose(output_file);
    return true;
}


bool deserialize(struct list** list, const char* filename){
	int i = 0, list_len, bytes;
	int* buf;

    FILE* input_file = fopen(filename, "rb");
    if (input_file == NULL) {
        return false;
    }
    fseek(input_file, 0, SEEK_END);
    bytes = ftell(input_file);
    list_len = bytes / sizeof(int);
    rewind(input_file);

    buf = (int*) malloc(bytes);
    fread(buf, bytes, 1, input_file);

    for (i = 0; i < list_len; i++) {
        list_add_front(list, buf[i]);
    }
    fclose(input_file);
    return true;
}