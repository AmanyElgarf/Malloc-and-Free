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
	int num;
	if (!(myblock[0]==(char)35&& myblock[1]==(char)111&&myblock[2]==(char)52&&myblock[3]==(char)241)) {	
		myblock[0]=(char)35;
		myblock[1]=(char)111;
		myblock[2]=(char)52;
		myblock[3]=(char)241;	//mymalloc is called first time, so we need to assign value to mybloc[0]
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
		entry* head;
		entry* newEntry;
		num = (int)myblock[4]*100+(int)myblock[5];	//largest available memory block
		if (num<(size+sizeof(entry))) {
			int memMinusMeta = num-sizeof(entry)>0 ? num-sizeof(entry) : 0 ;
			printf("Not enough memory. You have only %d bytes available\n", (int)(memMinusMeta)); 
			return NULL;
		} else {
			int ind = 6;	//this index for keeping track on what position in array myblock we currently are
			//here we need to start searching for the block of right size
			head = (entry*)&myblock[6];
			currEntry = head;
			while (1) {
				if (currEntry->blockSize>=(size+sizeof(entry)) && currEntry->free=='1') {
					if (currEntry->blockSize>=(size+sizeof(entry)+1)) {	// if block is big enough to split into 2 blocks
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
				// sleep(1);
					if ( currEntry->next!=NULL ) {	//there are more nodes ahead
						ind = ind + currEntry->blockSize+sizeof(entry);
						currEntry = currEntry->next;	
						// printf("Another entry is %d\n",anotherEntry->blockSize);
					} else {	//reached the end of the list. Check if have enough memory at the end
						anotherEntry = (entry*)&myblock[ind+sizeof(entry)+currEntry->blockSize];
						anotherEntry->next = currEntry->next;
						anotherEntry->free = '1'; 
						anotherEntry->next = NULL;
						int k = (int)currEntry->blockSize;
						int s = 4096-( ind + 2*(int)sizeof(entry)+k);
						anotherEntry->blockSize = s;
						if (anotherEntry->blockSize>=size) {  //check if need to split
							anotherEntry->free='0';
							if (anotherEntry->blockSize>(size+sizeof(entry) +1)) {
								anotherEntry->blockSize = size;
							}
							num = myblock[4]*100+myblock[5];
							num = num - (anotherEntry->blockSize+sizeof(entry));
							currEntry->next = anotherEntry;			//because of this linking, currEntry is now linked to anotherEntry
							myblock[4] = (char)(num/100);
							myblock[5] = (char)(num%100);
							// printf("")
							printf("location of %d bytes block is %d.\n",anotherEntry->blockSize,(int)(ind+sizeof(entry)*2+currEntry->blockSize));
							int p = (int)(ind+currEntry->blockSize+2*sizeof(entry));
							return &(myblock[p]);
						} else {
							printf("Could not find enough memory. Returning NULL\n");
							return NULL;
						}

					}
					
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
	malloc(30);
	entry* e = (entry*)&myblock[6];
	malloc(40);
	malloc(25);
	malloc(10);
	malloc(10);
	malloc(1500);
	printf("\nAll the allocated blocks are:\n");
	e = (entry*)&myblock[6];
	do {
		printf("size: %d\n",e->blockSize);
		e=e->next;
	} while (e!=NULL);
	return 0;
}
