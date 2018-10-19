#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

int main() {
	int e;
	struct list *list = NULL;

    printf("Enter list elements:\n");
    while (scanf("%d", &e) != EOF) { 
        list_add_front(&list, e);
    }
    list_info(list);
    return 0;
}