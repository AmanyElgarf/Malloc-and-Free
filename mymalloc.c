#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
	int num;
	static int firstCalled = 1;
	// if (!(myblock[0]==(char)35&& myblock[1]==(char)111&&myblock[2]==(char)52&&myblock[3]==(char)241)) {	
		// myblock[0]=(char)35;
		// myblock[1]=(char)111;
		// myblock[2]=(char)52;
		// myblock[3]=(char)241;	//mymalloc is called first time, so we need to assign value to mybloc[0]
	if (firstCalled) {
		firstCalled=0;
		printf("The size of entry is %d\n",(int)sizeof(entry));
		if (size>(4096-sizeof(entry)-6)) {
			printf("Cannot allocate that amount of space, try asking for less.\n");
			return 0;
		}
		num = 4096-size-sizeof(entry)-6;	
		myblock[4] = (char)(num/100);
		myblock[5] = (char)(num%100);
		entry first_entry;		//create first block , with metadata
		first_entry.blockSize= size;
		first_entry.free = '0';
		first_entry.next = NULL;
		*(entry*)(myblock+6) = first_entry;
		printf("Allocated %d bytes at %d position\n",(int)size,(int)(6+sizeof(entry)));
		return &myblock[6+sizeof(entry)]; // (or &myblock[5+sizeof(entry)])???? 
	} else {	//if we got here, than mymalloc was called before. Need to search whether we can find the block of 
				//free memory in myblock of needed size(including metadata size, which is sizeof(entry))
		entry* anotherEntry;
		entry* currEntry ;
		num = (int)myblock[4]*100+(int)myblock[5];	//largest available memory block
		printf("\n\nneed to allocate %d bytes, have %d bytes available\n",(int)size,num);
		if (num<(size+sizeof(entry))) {
			int memMinusMeta = num-sizeof(entry)>0 ? num-sizeof(entry) : 0 ;
			printf("Not enough memory. You have only %d bytes available\n", (int)(memMinusMeta)); 
			return NULL;
		} else {
			int ind = 6;	//this index for keeping track on what position in array myblock we currently are
			//here we need to start searching for the block of right size
			currEntry = (entry*)&myblock[6];
			while (1) {
				printf("currEntry's size is %d\n=========================\n",currEntry->blockSize);
				if (currEntry->blockSize>=(size+sizeof(entry)) && currEntry->free=='1') {
					//Found appropriate block,allocate memory within this block
					if (currEntry->blockSize>=(size+sizeof(entry)+1)) {	// if block is big enough to split into 2 blocks
						entry* newEntry ;	//this will be new block, with extra memory that we don't need now
						newEntry->blockSize = (currEntry->blockSize)-size-sizeof(entry);
						newEntry->free='1';
						newEntry->next = currEntry->next;
						currEntry->next = newEntry;
						currEntry->free = '0';
						num = myblock[4]*100+myblock[5];
						num =-(size+sizeof(entry));
						myblock[4] = (char)(num/100);
						myblock[5] = (char)(num%100);
						return &myblock[ind+sizeof(entry)];
					} else {	//block is not big enough for splitting
						currEntry->free='0';
						num = myblock[4]*100+myblock[5];
						num =-(currEntry->blockSize+sizeof(entry));
						myblock[4] = (char)(num/100);
						myblock[5] = (char)(num%100);
						return &myblock[ind+sizeof(entry)];
					}

				} else {
				sleep(1);
					if (currEntry->next!=NULL) {	//there are more nodes ahead
						ind = ind + currEntry->blockSize+sizeof(entry);
						currEntry = currEntry->next;
					} else {	//reached the end of the list. Check if have enough memory at the end
						printf("on line 82, currNode size is %d\n",currEntry->blockSize);
						anotherEntry->free = '1';
						anotherEntry->next = NULL;
						int k = (int)currEntry->blockSize;
						int s = 4096-( ind + 2*(int)sizeof(entry)+k);
						anotherEntry->blockSize = s;
						printf("currNode size changed to %d\n",currEntry->blockSize);
						if (anotherEntry->blockSize>=size) {  //check if need to split
							anotherEntry->free='0';
							if (anotherEntry->blockSize>(size+sizeof(entry) +1)) {
								anotherEntry->blockSize = size;
							}
							num = myblock[4]*100+myblock[5];
							num = num - (anotherEntry->blockSize+sizeof(entry));
							currEntry->next = anotherEntry;
							myblock[4] = (char)(num/100);
							myblock[5] = (char)(num%100);
							int p = (int)(ind+currEntry->blockSize+2*sizeof(entry));
							return &(myblock[p]);
						} else {
							printf("Could not find enough memory. Returning NULL\n");
							return NULL;
						}
					}
					// ind = ind + currEntry->blockSize+sizeof(entry);		// increasing ind too much(with every loop)
					// if (currEntry->next==NULL && (ind+size+sizeof(entry)<=4096)) { //if enough memory for the block:
					// //2 cases- split or not:
					// 	printf("at line 80, ind is %d\n",ind);
					// 	entry newEntry;
					// 	if (ind+size+sizeof(entry)*2+1<=4096) { //if can split
					// 		newEntry.blockSize = size;
					// 	} else {
					// 		newEntry.blockSize = 4096-ind-sizeof(entry);
					// 	}
					// 	newEntry.free='0';
					// 	newEntry.next = NULL;
					// 	currEntry->next = &newEntry;
					// 	printf("allocated successfully %d bytes! block is at %d position\n",(int)size,ind+(int)(sizeof(entry)));
					// 	printf("returning location: %d\n", ind +(int)sizeof(entry));
					// 	return &myblock[ind+sizeof(entry)];
					// } else  if (currEntry->next ==NULL && (ind+size+sizeof(entry)>4096)){ 	//not found block with enough memory
					// 	printf("Cannot allocate that much memory\n");
					// 	return 0;
					// } else {
					// 	currEntry = currEntry->next;
					// 	printf("ind is %d, currEntry blocksize is %d\n", ind,(int)currEntry->blockSize);
					// 		// ind = ind + currEntry->blockSize +sizeof(entry);
					// 	printf("have to move to next node, update ind to %d\n",ind);
					// 	printf("size is %d\n",(int)size);
					// }
						// return 0;
				} 
			}
		}
		
	}
	return 0;
}
int main(int argc, char* argv[]) {
	int *a;
	a = malloc(3060);
	int x = (int)myblock[4]*100+(int)myblock[5];
	printf("there are %d bytes left after first allocation\n",x);
	malloc(30);
	malloc(50);
	malloc(55);
	// malloc(10);
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
