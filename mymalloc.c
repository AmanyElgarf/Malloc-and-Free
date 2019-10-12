#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"
// struct is compressed for space efficiency, so there is no padding, and it takes only 13 bytes of code
//'free' variable is of type char - to take as little memory as possible
typedef struct __attribute__((__packed__)) _entry {		//this struct will hold metadata, about each allocated block
	char free;
	int blockSize;
	struct _entry *next;	
}entry;


void myfree(void *ptr){
    
    //freeing a pointer that wasn't allocated by malloc before
    entry* b = (entry*)(((char*)ptr));
    if ((char*)b < (myblock+6) || (char*)b >= myblock+sizeof(myblock)){
        printf("Error: (%p) is either not a pointer or a pointer that  was not allocated by malloc before.\n", &ptr);
        return;
    }
    
    //freeing a pointer that was already freed
    if(b->free == '1'){
        printf("Error: pointer (%p) was freed before.\n", ptr);
        return;
    }
    else{
        if(b->blockSize > 4090){
            printf("Error: You can't free a pointer of size more than 4090 because we are using the first six units to store inf\n");
            return;
        }
        entry* head = (entry*)(((char*)&myblock[6]));
        
        //add block space to total space
        myblock[4] = (char)((int)myblock[4] + b->blockSize/100);
        myblock[5] = (char)((int)myblock[5] + b->blockSize%100);
        b->free = '1';
        //checck for next block
        if(b->next != NULL){
            if(b->next->free == '1'){
                b->blockSize = b->blockSize + b->next->blockSize;
                if(b->next->next != NULL){
                    b->next =b->next->next;
                }
                else{
                    b->next = NULL;
                }
         }
        }
        
        entry* curr = head;
        printf("%p\n", &myblock[0]);
        printf("%p\n", head);
        printf("%p\n", b);
        printf("%p\n", head->next);



        if(curr != b){
            while(curr->next != b){
                printf("%s", "hi");
                printf("%p\n", curr);
                curr = curr->next;
            }
            if(curr->free =='1'){
                curr->blockSize = curr->blockSize + b->blockSize;
                if(curr->next->next != NULL){
                    curr->next =curr->next->next;
                }
                else{
                    curr->next = NULL;
                }
            }
        }
        
        entry* check = head;
        int count = 0;
        while(check!=NULL){
            if(check->free == '0'){
                count++;
            }
            check=check->next;
        }
        if(count == 0){
            myblock[0]=(char)345;
            myblock[1]=(char)1441;
            myblock[2]=(char)524;
            myblock[3]=(char)24;
        }
        printf("Pointer (%p) was freed successfully.\n", ptr);
    }
    
}



void *mymalloc(size_t size,char* filename,int lineNumber) {
	printf("allocating %d bytes\n",(int)size);
	int num;
	if (!(myblock[0]==(char)35&& myblock[1]==(char)111&&myblock[2]==(char)52&&myblock[3]==(char)241)) {	
		myblock[0]=(char)35;
		myblock[1]=(char)111;
		myblock[2]=(char)52;
		myblock[3]=(char)241;	//mymalloc is called first time, so we need to assign value to mybloc[0]
		if (size>(4096-sizeof(entry)-6)) {
			printf("Cannot allocate that amount of space, try asking for less.\n");
			return NULL;
		} else if (size==0) {
			printf("cannot allocate for 0 bytes\n");
			return NULL;
		}
		num = 4096-size-sizeof(entry)-6;	//'num' holds total amount of available memory
		myblock[4] = (char)(num/100);		//splitting and storing num in 5th and 6th elements of array
		myblock[5] = (char)(num%100);
		entry first_entry;		//create metadata for first block 
		first_entry.blockSize= size;
		first_entry.free = '0';
		first_entry.next = NULL;
		*(entry*)(myblock+6) = first_entry;
		printf("Allocated %d bytes at %d position\n",(int)size,(int)(6+sizeof(entry)));
		return &myblock[6+sizeof(entry)]; // (or &myblock[5+sizeof(entry)])???? 
	} else {	//if we got here, than mymalloc was called before. Will search for large enough block of 
				//free memory (including metadata size, which is sizeof(entry))
		entry* anotherEntry;
		entry* currEntry ;
		entry* newEntry;
		num = (int)myblock[4]*100+(int)myblock[5];	//largest available memory block
		if (num<(size+sizeof(entry))) {	//in case user asks too much
			int memMinusMeta = num-sizeof(entry)>0 ? num-sizeof(entry) : 0 ;
			printf("Not enough memory. You have only %d bytes available\n", (int)(memMinusMeta)); 
			return NULL;
		} else if (size==0) {	//in case if user asks 0 
			printf("Cannot allocate 0 bytes\n");
			return NULL;
		}else {
			int ind = 6;	//this index for keeping track on what position in array myblock we currently are
			//here we need to start searching for the block of right size
			currEntry = (entry*)&myblock[6];	//create currEntry for traversing through the metadata
			while (1) {
				printf("curr entry's size is %d, free value is %c\n",currEntry->blockSize,currEntry->free);
				if (currEntry->blockSize>=(size+sizeof(entry)) && currEntry->free=='1') {	//if found large enough block that was freed before
					printf("line 136\n");
					if (currEntry->blockSize>=(size+sizeof(entry)+1)) {	// check if block is big enough to split into 2 blocks
						newEntry = (entry*)&myblock[ind+sizeof(entry)+size];					
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
					if ( currEntry->next!=NULL ) {	//there are more nodes ahead
						ind = ind + currEntry->blockSize+sizeof(entry);
						currEntry = currEntry->next;	
					} else {	//reached the end of the list. Check if have enough memory at the end
						anotherEntry = (entry*)&myblock[ind+sizeof(entry)+currEntry->blockSize];
						anotherEntry->next = currEntry->next;
						anotherEntry->free = '1'; 
						int k = (int)currEntry->blockSize;
						int s = 4096-( ind + 2*(int)sizeof(entry)+k);
						anotherEntry->blockSize = s;
						if (anotherEntry->blockSize>=size) {  //check if need to split
							anotherEntry->free='0';
							if (anotherEntry->blockSize>(size+sizeof(entry) +1))  { anotherEntry->blockSize = size; }
							num = myblock[4]*100+myblock[5];
							num = num - (anotherEntry->blockSize+sizeof(entry));
							currEntry->next = anotherEntry;			//because of this linking, currEntry is now linked to anotherEntry
							myblock[4] = (char)(num/100);
							myblock[5] = (char)(num%100);
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
	return NULL;
}
int main(int argc, char* argv[]) {
	int *a;
	a = malloc(300);
	// int x = (int)myblock[4]*100+(int)myblock[5];
	malloc(30);
	entry* e = (entry*)&myblock[6];
	int* aa = (int*)malloc(40);
	*(aa+1) = 5;
	*(aa+3)=765;
	printf("a[1] is %d, aa[3] is %d\n",*(aa+1),*(aa+3));
	malloc(100);
	malloc(100);
	malloc(100);
	malloc(150);
	malloc(100);
	malloc(0);
	printf("\nAll the allocated blocks are:\n");
	e = (entry*)&myblock[6];
	do {
		printf("size: %d\n",e->blockSize);
		e=e->next;
	} while (e!=NULL);
	myfree(a);
	a = malloc(100);
	// myfree(a);
	// int b = 3;
	// myfree((int*)b);
	return 0;
}
