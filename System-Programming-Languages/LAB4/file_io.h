#ifndef Lab4_file_io
#define Lab4_file_io

typedef int bool;
#define true 1
#define false 0


bool load(struct list** list, const char* filename);


bool save(struct list* list, const char* filename);


bool serialize(struct list* list, const char* filename);


bool deserialize(struct list** list, const char* filename);


#endif 