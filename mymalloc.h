#ifndef _mymalloc_h_
#define _mymalloc_h_
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>


#define malloc(x) mymalloc((size_t)x,__FILE__,__LINE__)
#define free(x) myfree(x,__FILE__,__LINE__)
#define SizeOfEntry sizeof(entry)

typedef struct __attribute__((__packed__)) _entry {		
	char free;
	int blockSize;
	struct _entry *next;
	void* dataPtr;	
}entry;

void *mymalloc(size_t size, char *filename, int line);

void myfree(void *p, char *filename, int line);

#endif