#ifndef Lab4_higher_order
#define Lab4_higher_order

struct list* map (struct list* list, int(*f) (int));


void map_mut (struct list* list, int(*f) (int));


void foreach (struct list* list, void(*f) (int));


int foldl (struct list* list, int acc, int(*f) (int, int));


struct list* iterate (int length, int init_value, int(*f)(int));

#endif 