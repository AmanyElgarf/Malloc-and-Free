#ifndef _mymalloc_h
#define _mymalloc_h
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>


#define malloc(x) mymalloc((size_t)x,__FILE__,__LINE__)
#define free(x) myfree(x,__FILE__,__LINE__)
#define SizeOfEntry sizeof(entry)
#define sizeOfArray 4096
static char myblock[sizeOfArray];

// void *mymalloc(size_t size, char* filename, int lineNumber);
void *mymalloc(size_t size,char* filename, int lineNumber);
void myfree(void *ptr);
#endif