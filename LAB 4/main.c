#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "linked_list.h"
#include "file_io.h"
#include "list_higher_order.h"


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
        	puts("Enter file name:");
        	scanf("%63s", filename);
            load(&list, "filename");
            break;
        }
        case 2: {
        	puts("Enter list elements delimited by whitespace and finish the input with ^D");
    		while (scanf("%d", &e) != EOF) { 
        		list_add_front(&list, e);
    		}
            break;
        }
        default: {
            printf("Unknown command\n");
            return 0;
        }
    }

    
    puts("");
    list_info(list);
    puts("\nList elements in a row:");
    foreach(list, print_space);
    puts("\nList elements in a collumn:");
    foreach(list, print_newline);
    puts("Min element:");
    printf("%d\n", foldl(list, INT_MAX, min));
    puts("Max element:");
    printf("%d\n", foldl(list, INT_MIN, max));
    puts("Sum:");
    printf("%d\n", foldl(list, 0, sum));
    puts("Squares:");
    foreach(map(list, square), print_space);
    puts("\nCubes:");
    foreach(map(list, cube), print_space);
    puts("\nModules:");
    map_mut(list, abs);
    foreach(list, print_space);
    puts("\nFirst 10 powers of 2:");
    foreach(iterate(10, 1, mul_by2), print_space);
    puts("");


    return 0;
}