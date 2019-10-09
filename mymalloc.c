#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
typedef struct __attribute__((__packed__)) _entry {		//this struct will hold metadata, about each allocated block
	char free;
	int blockSize;
	struct _entry *next;	
}entry;
void *mymalloc(size_t size,char* filename,int lineNumber) {
	//before we allocate memory, we need to check whether malloc is called for the first time
	//for that, we use first char in myblock, myblock[0] to store some value, that will indicate that mymalloc 
	// was called before already
	if (myblock[0]!=(char)35) {	
		myblock[0]=(char)35;	//mymalloc is called first time, so we need to assign value to mybloc[0]
		printf("The size of entry is %d\n",(int)sizeof(entry));
		if (size>(4096-sizeof(entry)-9)) {
			printf("Cannot allocate that amount of space, try asking for less.\n");
			return 0;
		}
		int num = 4096-size-2*sizeof(entry)-3;		//num is the size of the largest available block of memory, 
		//it is split between myblock[1] and myblock[2]
		myblock[1] = (char)(num/100);
		myblock[2] = (char)(num%100);
		entry first_entry;		//create first block , with metadata
		first_entry.blockSize= size;
		first_entry.free = '0';
		first_entry.next = NULL;
		// *(struct thing*)(heap + offset) = test;
		*(entry*)(myblock+3) = first_entry;

		return &myblock[3+sizeof(entry)]; // (or &myblock[5+sizeof(entry)])???? 
	} else {	//if we got here, than mymalloc was called before. Need to search whether we can find the block of 
				//free memory in myblock of needed size(including metadata size, which is sizeof(entry))
		int num = (int)myblock[1]*100+(int)myblock[2];	//largest available memory block
		printf("need to allocate %d bytes, have %d bytes available\n",(int)size,num);
		// if (num<(size+sizeof(entry))) printf("Not enough memory. You can ask at most for %d bytes\n", (int)(num-sizeof(entry))); 
		// else {
			int ind = 3;	//this index for keeping track on what position in array myblock we currently are
			//here we need to start searching for the block of right size
			entry *currEntry = (entry*)(myblock+3);
			while (1) {
				if (currEntry->blockSize>=(size+sizeof(entry)) && currEntry->free=='1') {
					//Found appropriate block,allocate memory within this block
					if (currEntry->blockSize>(size+2*sizeof(entry))) {	// if block is big enough to split into 2 blocks
						entry* newEntry ;	//this will be new block, with extra memory that we don't need now
						newEntry->blockSize = (currEntry->blockSize)-size-sizeof(entry);
						newEntry->free='1';
						newEntry->next = currEntry->next;
						currEntry->next = newEntry;
						num = myblock[1]*100+myblock[2];
						num =-(size+2*sizeof(entry));
						myblock[1] = (char)(num/100);
						myblock[2] = (char)(num%100);
						printf("allocated %d bytes ,line 56\n",(int)size);
						return &myblock[3+sizeof(entry)];
						//need to finish this part, split block into two blocks. Remember to update index
					} else {	//block is not big enough for splitting
						currEntry->free='0';
						num = myblock[1]*100+myblock[2];
						num =-(currEntry->blockSize+2*sizeof(entry));
						myblock[1] = (char)(num/100);
						myblock[2] = (char)(num%100);
						printf("allocated %d bytes ,line 65\n",(int)size);
						if (currEntry->next==NULL) printf("next is null\n");
						return &myblock[ind+sizeof(entry)];
					}

				} else  {
					if (currEntry->free!='1' || currEntry->blockSize<size+sizeof(entry)) {
						ind = ind + currEntry->blockSize+sizeof(entry);
						printf("%d,line 71\n",(ind + (int)size +(int)sizeof(entry)));
						if (currEntry->next==NULL && (ind+size+sizeof(entry)<=4096)) { //if enough memory for the block:
							//2 cases- split or not:
							entry newEntry;
							if (ind+size+sizeof(entry)*2+1<=4096) { //if can split
								newEntry.blockSize = size;
							} else {
								newEntry.blockSize = 4096-ind-sizeof(entry);
							}
							newEntry.free='0';
							newEntry.next = NULL;
							currEntry->next = &newEntry;
							return &myblock[ind+sizeof(entry)];
						} else  if (currEntry->next ==NULL && (ind+size+sizeof(entry)>4096)){ 	//not found block with enough memory
							printf("Cannot allocate that much memory\n");
							return 0;
						} else {
							currEntry = currEntry->next;
							printf("ind is %d\n", ind);
							ind = ind + currEntry->blockSize+sizeof(entry);
							printf("have to move to next node, update ind to %d\n",ind);
						}
						// return 0;
					}

				 //if next node is NULL- no more blocks
					// if (currEntry->next==NULL &&(ind+size+sizeof(entry)<=4095)) {
					// 	printf("ind is %d, line 70\n",ind);
					// 	ind = ind + currEntry->blockSize +sizeof(entry);
					// 	printf("ind is %d,line 72\n",ind);
					// 	entry newNode ;
					// 	newNode.blockSize = size;
					// 	newNode.free='0';
					// 	newNode.next=NULL;
					// 	currEntry->next = &(newNode);
					// 	printf("block is at index %d\n",(int)(ind+sizeof(entry)));

					// 	//need to change num here:
					// 	num = num - (size+sizeof(entry));
					// 	return &myblock[ind+sizeof(entry)];
					// }else if (currEntry->next==NULL && (ind+size+sizeof(entry)>4095)) {
					// 	printf("Cannot find enough memory, reached the end of array\n");
					// } else {	//block is too small, or not free, so going to the next one
					// 	ind = ind +(currEntry->blockSize)+sizeof(entry);
					// 	printf("updating index, it is now %d\n",ind);
					// 	currEntry = currEntry->next;
					// }
				} 
			}
		// }
		
	}
	return 0;
}
int main(int argc, char* argv[]) {
	int *a;
	a = malloc(1000);
	int x = myblock[1]*100+myblock[2];
	printf("there are %d bytes left after first allocation\n",x);
	malloc(3065);
	malloc(50);
	// malloc(194);
	return 0;
}
// char mychars[10];
// int * intlocation = (int*)(&mychar[5]);
// *intlocation = 3632;

// struct thing {
//     int val;
// };
// char heap[64];
// int offset = 27;
// struct thing test = {3};

// // Assigning
// *(struct thing*)(heap + offset) = test;

// // Accessing
// struct thing *ptr = (struct thing*)(heap + offset);
// printf("%d", ptr->val); // 3
