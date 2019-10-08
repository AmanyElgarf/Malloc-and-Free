#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
typedef struct __attribute__((__packed__)) _entry {		//this struct will hold metadata, about each allocated block
	char free;
	size_t blockSize;
	struct entry *next;	
}entry;
void *mymalloc(size_t size,char* filename,int lineNumber) {
	//before we allocate memory, we need to check whether malloc is called for the first time
	//for that, we use first char in myblock, myblock[0] to store some value, that will indicate that mymalloc 
	// was called before already
	if (myblock[0]!=(char)213) {	
		myblock[0]=(char)213;	//mymalloc is called first time, so we need to assign value to mybloc[0]
		printf("The size of entry is %d\n",(int)sizeof(entry));
		if (size>(4096-sizeof(entry)-9)) {
			printf("Cannot allocate that amount of space, try asking for less.\n");
			return 0;
		}
		int num = 4096-size-3;
		myblock[1] = (char)(num/100);
		myblock[2] = (char)(num%100);
		entry first_entry;		//create first block , with metadata
		first_entry.blockSize= size;
		first_entry.free = '0';
		first_entry.next = NULL;
		return &myblock[5+sizeof(entry)];
	} else {	//if we got here, than mymalloc was called before. Need to search whether we can find the block of 
				//free memory in myblock of needed size(including metadata size, which is sizeof(entry))
		printf("it is now %c\n",myblock[0]);
	}
	
	return 0;
}
int main(int argc, char* argv[]) {
	int *a;
	a = malloc(1234);
	int x = myblock[1]*100+myblock[2];
	printf("there are %d bytes left after first allocation\n",x);
	malloc(97);
	return 0;
}
// char mychars[10];
// int * intlocation = (int*)(&mychar[5]);
// *intlocation = 3632;