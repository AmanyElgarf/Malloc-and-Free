#include <stdio.h>
#include <stddef.h>

#define malloc(x) mymalloc(x,__FILE__,__LINE__)
#define free(x) myfree(x,__FILE__,__LINE__)
#define SizeOfEntry sizeof(entry)
char myblock[4096];

// void *mymalloc(size_t size, char* filename, int lineNumber);
void *mymalloc(size_t size,char* filename, int lineNumber);
void myfree(void *ptr);
