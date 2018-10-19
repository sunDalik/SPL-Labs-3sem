#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

struct list* map (struct list* list, int(*f) (int)) {
	struct list* new_list = NULL;
	while (list != NULL) {
        list_add_back(&new_list, f(list->element));
        list = list->succ;
    }
	return new_list;
}

int cube (int x) {
	return x*x*x;
}

int square (int x) {
	return x*x;
}

int main() {
	int e;
	struct list *list = NULL;

    printf("Enter list elements:\n");
    while (scanf("%d", &e) != EOF) { 
        list_add_front(&list, e);
    }
    list_info(list);
    list_info(map(list, cube));
    return 0;
}