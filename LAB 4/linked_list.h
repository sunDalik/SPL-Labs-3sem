#ifndef Lab4_linked_list
#define Lab4_linked_list


typedef struct list list;

/**
 *  Linked list structure 
 *  List containts integer element and pointer to the next element
 */
struct list {
    int element;
    struct list* succ; /* successor */
};

/**
 *  Create list with one element
 *  @param e - first element
 *  @return pointer to the list
 */
struct list* list_create(int e);

/**
 *  Add element to the begining of a list
 *  Creates it if the list is empty
 *  @param list - pointer to the list
 *  @return pointer to the list
 */
void list_add_front(struct list** listptr, int e);

/**
 *  Add element to the end of a list
 *  Creates it if the list is empty
 *  @param list - pointer to the list
 *  @return pointer to the list
 */
void list_add_back(struct list** listptr, int e);

/**
 *  Print list length, sum and all its elements
 *  @param list - pointer to the list
 */
 void list_info(struct list* list);

/**
*  Get element by index
*  @param list - pointer to the list
*  @param index - element index
*  @return element or null if index is out of bounds
*/
struct list* list_node_at(struct list* list, unsigned int index);

/**
 *  Delete all list elements
 *  @param list - pointer to the list
*/
void list_free(struct list** list);

/**
 *  Get list length
 *  @param  list - pointer to the list
 *  @return list length
 */
 int list_length(struct list* list);

/**
*  Get value of element by index
*  @param list - pointer to the list
*  @param index - element index
*  @return element value by index
*/
int list_get(struct list* list, unsigned int index);

/**
 *  Sum all list elements
 *  @param list - pointer to the list
 *  @return list sum
 */
int list_sum(struct list* list);
#endif 