#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"
// struct is compressed for space efficiency, so there is no padding, and it takes only 13 bytes of code
//'free' variable is of type char - to take as little memory as possible
typedef struct __attribute__((__packed__)) _entry {        //this struct will hold metadata, about each allocated block
    char free;
    int blockSize;
    void* dataPtr;
    struct _entry *next;
}entry;


void myfree(void *ptr){
    
    //freeing a pointer that wasn't allocated by malloc before
    if ((char*)ptr < (myblock+6) || (char*)ptr >= myblock+sizeof(myblock)){
        printf("Error: (%p) is either not a pointer or a pointer that  was not allocated by malloc before.\n", ptr);
        return;
    }
    
    entry* head;
    head = (entry*)&myblock[6];
    entry* new = head;
    while(new->dataPtr != ptr){
        new = new->next;
    }
   
    //freeing a pointer that was already freed
    if(new->free == '1'){
        printf("Error: pointer (%p) was freed before.\n", ptr);
        return;
    }
    
    
    else{
        if(new->blockSize > 4090){
            printf("Error: You can't free a pointer of size more than 4090 because we are using the first six units to store inf\n");
            return;
        }
        
        //add block space to total space
        myblock[4] = (char)((int)myblock[4] + head->blockSize/100);
        myblock[5] = (char)((int)myblock[5] + head->blockSize%100);
        new->free = '1';
        //checck for next block
        if(new->next != NULL){
            if(new->next->free == '1'){
                new->blockSize = new->blockSize + new->next->blockSize;
                if(new->next->next != NULL){
                    new->next =new->next->next;
                }
                else{
                    new->next = NULL;
                }
         }
        }
        
        entry* curr = head;

        if(curr != new){
            while(curr->next != new){
             
                curr = curr->next;

            }
            if(curr->free =='1'){
                curr->blockSize = curr->blockSize + new->blockSize;
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
	int num;
	printf("mallocing %d bytes\n",(int)size);
	if (!(myblock[0]==(char)35&& myblock[1]==(char)111&&myblock[2]==(char)52&&myblock[3]==(char)241)) {	
		myblock[0]=(char)35;
		myblock[1]=(char)111;
		myblock[2]=(char)52;
		myblock[3]=(char)241;	//mymalloc is called first time, so we need to assign value to mybloc[0]
		printf("The size of entry is %d\n",(int)sizeof(entry));
		if (size>(4096-sizeof(entry)-6)) {
			printf("Cannot allocate that amount of space, try asking for less.\n");
			return NULL;
		} else if (size==0) {
			printf("%s: %d: error:\tCannot allocate for 0 bytes\n",filename,lineNumber);
			return NULL;
		}
		printf("line27\n");
		num = 4096-size-sizeof(entry)-6;	
		myblock[4] = (char)(num/100);
		myblock[5] = (char)(num%100);
		entry first_entry;		//create first block , with metadata
		first_entry.blockSize= size;
		first_entry.free = '0';
		first_entry.next = NULL;
		first_entry.dataPtr = &myblock[19];
		*(entry*)(myblock+6) = first_entry;
		printf("Allocated %d bytes at %d position\n",(int)size,(int)(6+sizeof(entry)));
		return first_entry.dataPtr;
		// return &myblock[6+sizeof(entry)]; // (or &myblock[5+sizeof(entry)])???? 
	} else {	//if we got here, than mymalloc was called before. Need to search whether we can find the block of 
				//free memory in myblock of needed size(including metadata size, which is sizeof(entry))
		entry* anotherEntry;
		entry* currEntry ;
		entry* newEntry;
		num = (int)myblock[4]*100+(int)myblock[5];	//largest available memory block
		if (num<(size+sizeof(entry))) {
			int memMinusMeta = num-sizeof(entry)>0 ? num-sizeof(entry) : 0 ;
			printf("Not enough memory. You have only %d bytes available\n", (int)(memMinusMeta)); 
			return NULL;
		} else if (size==0) {	//in case if user asks 0 
			printf("%s: %d: error:\tCannot allocate 0 bytes\n",filename, lineNumber);
			return NULL;
		}else {
			printf("line55\n");
			int ind = 6;	//this index for keeping track on what position in array myblock we currently are
			//here we need to start searching for the block of right size
			currEntry = (entry*)&myblock[6];
			while (1) {
				printf("line59\n");
				if (currEntry->blockSize>=(size+sizeof(entry)) && currEntry->free=='1') {

					if (currEntry->blockSize>=(size+sizeof(entry)+1)) {	// if block is big enough to split into 2 blocks
						printf("line62\n");
						printf("currEntry blocksize is %d\n",currEntry->blockSize);
						int a = currEntry->blockSize-(int)size-sizeof(entry);
						printf("a is %d\n",a);
						newEntry = (entry*)&myblock[ind+sizeof(entry)+size];
						newEntry->blockSize = a;
						printf("line 73\n");
						newEntry->free='1';
						newEntry->next = currEntry->next;
						// newEntry->dataPtr = &myblock[ind+sizeof(entry)];	????????
						newEntry->dataPtr = &myblock[ind+sizeof(entry)*2 +size];
						currEntry->blockSize = size;
						currEntry->next = newEntry;
						currEntry->free = '0';
						num = myblock[4]*100+myblock[5];
						num =-(size+sizeof(entry));
						myblock[4] = (char)(num/100);
						myblock[5] = (char)(num%100);
						// return newEntry->dataPtr;
						return currEntry->dataPtr;
					} else {	//block is not big enough for splitting
						currEntry->free='0';
						num = myblock[4]*100+myblock[5];
						num =-(currEntry->blockSize+sizeof(entry));
						myblock[4] = (char)(num/100);
						myblock[5] = (char)(num%100);
						currEntry->dataPtr = &myblock[ind+sizeof(entry)];
						printf("returning pointer %p\n",newEntry->dataPtr);
						return currEntry->dataPtr;
					}

				} else {
					if ( currEntry->next!=NULL ) {	//there are more nodes ahead
						ind = ind + currEntry->blockSize+sizeof(entry);
						currEntry = currEntry->next;	
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
							printf("location of %d bytes block is %d.\n",anotherEntry->blockSize,(int)(ind+sizeof(entry)*2+currEntry->blockSize));
							int p = (int)(ind+currEntry->blockSize+2*sizeof(entry));
							anotherEntry->dataPtr = &(myblock[p]);
							printf("returning pointer %p\n",anotherEntry->dataPtr);
							return anotherEntry->dataPtr;
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
