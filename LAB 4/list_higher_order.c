#include <string.h>
#include "list_higher_order.h"
#include "linked_list.h"


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