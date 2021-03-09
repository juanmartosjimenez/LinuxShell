//if not defined 
#ifndef LIST_H
#define LIST_H

// empty list is a null pointer
// list cell has an owned copy of the string it contains
typedef struct list {
    char* head; //owned copy
    struct list* tail; //takes ownership

} list;

list* cons(char* xx, list* xs);
void free_list(list* xs);
void print_list(list* xs);
long length(list* xs);
list* reverse(list* xs);
list* rev_free(list* xs);


#endif 
