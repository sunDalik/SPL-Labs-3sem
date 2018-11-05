#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "linked_list.h"

#define true 1
#define false 0

typedef int bool;

struct list* map (struct list* list, int(*f) (int)) {
	struct list* new_list = NULL;
	while (list != NULL) {
        list_add_back(&new_list, f(list->element));
        list = list->succ;
    }
	return new_list;
}

void map_mut (struct list* list, int(*f) (int)) {
	while (list != NULL) {
		list->element = f(list->element);
        list = list->succ;
    }
}

void foreach (struct list* list, void(*f) (int)) {
	while (list != NULL) {
        f(list->element);
        list = list->succ;
    }
}

int foldl (struct list* list, int acc, int(*f) (int, int)){
    while (list != NULL) {
        acc = f(list-> element, acc);
        list = list->succ;
    }
    return acc;
}

struct list* iterate (int length, int init_value, int(*f)(int)){
    struct list* new_list = list_create(init_value);
    int i;
    for(i = 0; i < length; i++){
        init_value = f(init_value);
        list_add_back(&new_list, init_value);
    }
    return new_list;
}








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

int abs (int x) { return x < 0? -x: x; }
int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }
int sum (int a, int b) { return a+b;}
int cube (int x) { return x*x*x; }
int square (int x) { return x*x; }
void print_space (int x) { printf("%d ", x); }
void print_newline (int x) { printf("%d\n", x); }
int mul_by2(int x) { return x*2; }

int main() {
	int e;
	struct list *list = NULL;
	int choice;
	char filename[64];

	puts("Enter 1 to read from file or 2 to read from stdin");
    scanf("%d", &choice);
    switch (choice){
        case 1: {
        	puts("Enter file name:\n");
        	scanf("%63s", filename);
            load(&list, "filename");
            break;
        }
        case 2: {
        	puts("Enter list elements:\n");
    		while (scanf("%d", &e) != EOF) { 
        		list_add_front(&list, e);
                puts("qqqqq");
    		}
            break;
        }
        default: {
            printf("Unknown command");
            return 0;
        }
    }

    
    
    list_info(list);
    foreach(list, print_space);
    foreach(list, print_newline);
    list_info(map(list, square));
    list_info(map(list, cube));
    map_mut(list, abs);
    list_info(iterate(10, 1, mul_by2));
    printf("%d\n", foldl(list, 0, sum));
    printf("%d\n", foldl(list, INT_MAX, min));
    printf("%d\n", foldl(list, INT_MIN, max));

    return 0;
}