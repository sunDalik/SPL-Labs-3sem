#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

struct list* list_create(int e) {
    struct list *new_list = (struct list*)malloc(sizeof(struct list));
    new_list->element = e;
    new_list->succ = NULL;
    return new_list;
} 

 void list_add_front(struct list** listptr, int e) {
    if (*listptr == NULL){
        *listptr = list_create(e);
    }
    else {
        list* next = *listptr;
        *listptr = list_create(e);
        (*listptr)->succ = next;
    }
} 

void list_add_back(struct list** listptr, int e){
    list* new_node;
    if (*listptr == NULL){
        *listptr = list_create(e);
    }
    else {
        list* list = *listptr;
        new_node = list_create(e);
        while (list->succ != NULL) list = list->succ;
        list->succ = new_node;
    }
}

void list_info(struct list* list){
    printf("List length: %d\n", list_length(list));
    printf("List sum: %d\n", list_sum(list));
    printf("List elements: ");
    while (list != NULL){
        printf("%d, ", list->element);
        list = list->succ;
    }
}

struct list* list_node_at(struct list* list, unsigned int index) {
    int i;

    for (i = 0; i < index; i++) {
        list = list->succ;
        if (list == NULL) break;
    }

    return list;
}

void list_free(struct list** listptr){
    list* list1 = *listptr;
    list* list2;
    while (list1 != NULL){
        list2 = list1->succ;
        free(list1);
        list1 = list2;
    } 
    *listptr = NULL;
}

int list_length(struct list* list){
    int i;
    for (i = 0; list != NULL; i++) list = list->succ;
    return i;
}

int list_get(struct list* list, unsigned int index){
    struct list* node = list_node_at(list, index);
    return node == NULL ? 0 : node->element;
}

int list_sum(struct list* list){
    int sum = 0;
    while (list != NULL) {
        sum += list->element;
        list = list->succ;
    }
    return sum;
}
