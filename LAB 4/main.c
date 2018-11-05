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
        	puts("Enter list elements:");
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