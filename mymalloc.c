#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"
#include <time.h>

#define sizeOfArray 4096
static char myblock[sizeOfArray];

int isInitialized() {
	int result = 0;
	if (myblock[0]==(char)35&& myblock[1]==(char)111&&myblock[2]==(char)52&&myblock[3]==(char)241) result = 1;
	return result;
}

void initialize() {
	myblock[0]=(char)35;
	myblock[1]=(char)111;
	myblock[2]=(char)52;
	myblock[3]=(char)241;
	return;

}
void myfree(void *ptr,char* filename,int lineNumber){
    //freeing a pointer that wasn't allocated by malloc before
    int freedBytes = 0;
    if ((char*)ptr < (myblock+6) || (char*)ptr >= myblock+sizeof(myblock)){
        printf("%s: %d: Error: (%p) is either not a pointer or a pointer that  was not allocated by malloc before.\n", filename,lineNumber,ptr);
        return;
    }
    entry* head;
    head = (entry*)&myblock[6];
    entry* neww = head;
    //checking if value passed is pointer to dataBlock
    while(neww->dataPtr != ptr && neww->next != NULL){ neww = neww->next; } 
    if(neww->next == NULL && neww->dataPtr != ptr){
        printf("%s: %d: Error: (%p) is either not a pointer or a pointer that  was not allocated by malloc before.\n", filename,lineNumber,ptr);
        return;
    }
    //freeing a pointer that was already freed
    if(neww->free == '1'){
        printf("%s: %d: Error: pointer (%p) was freed before.\n",filename,lineNumber, ptr);
        return;
    }
    else{
        if(neww->blockSize > 4069){
            printf("%s: %d: Error: You can't free a pointer of size more than 4090 because we are using the first six units to store inf\n",filename,lineNumber);
            return;
        }
        //add block space to total space
        freedBytes = freedBytes+neww->blockSize;
        neww->free = '1';
        //check for next block
        if(neww->next != NULL){
            if(neww->next->free == '1'){
                neww->blockSize = neww->blockSize + neww->next->blockSize+sizeof(entry) ;
                freedBytes = neww->blockSize;
                if(neww->next->next != NULL){ neww->next =neww->next->next; }
                else{ neww->next = NULL; }
         	}
        }
        entry* curr = head;
        if(curr != neww){
            while(curr->next != neww){ curr = curr->next; }
            if(curr->free =='1'){
                curr->blockSize = curr->blockSize + neww->blockSize+sizeof(entry);
                freedBytes = curr->blockSize;
                if(curr->next->next != NULL){ curr->next =curr->next->next; }
                else{ curr->next = NULL;  }
            }
        }        
    }    
}


void *mymalloc(size_t size,char* filename,int lineNumber) {
	int k,s;
	if (isInitialized()==0) {		////mymalloc is called first time, so we need to assign value to mybloc[0]
		initialize();
		if (size>(4096-sizeof(entry)-6)) {
			printf("%s: %d: Cannot allocate that amount of space, try asking for less.\n",filename,lineNumber);
			return NULL;
		} else if (size==0) {
			printf("%s: %d: error:\tCannot allocate for 0 bytes\n",filename,lineNumber);
			return NULL;
		}
		entry first_entry ;		//create first block , with metadata
		first_entry.blockSize= size;
		first_entry.free = '0';
		first_entry.next = NULL;
		first_entry.dataPtr = &myblock[6+sizeof(entry)];
		*(entry*)(myblock+6) = first_entry;
		return first_entry.dataPtr;
	} else {	//if we got here, than mymalloc was called before. Need to search whether we can find the block of 
		entry* anotherEntry;
		entry* currEntry ;
		entry* newEntry;
		if (size==0) {	//in case if user asks 0 
			printf("%s: %d: error:\tCannot allocate 0 bytes\n",filename, lineNumber);
			return NULL;
		}else {
			int ind = 6;	//this index for keeping track on what position in array myblock we currently are
			currEntry = (entry*)&myblock[6];
			while (1) {
				if (currEntry->blockSize>=(int)(size) && currEntry->free=='1') {
					if (currEntry->blockSize>=(int)(size+sizeof(entry)+1)) {	// if block is big enough to split into 2 blocks
						int a = currEntry->blockSize-(int)size-sizeof(entry);
						newEntry = (entry*)&myblock[ind+sizeof(entry)+size];
						newEntry->blockSize = a;
						newEntry->free='1';
						newEntry->next = currEntry->next;
						newEntry->dataPtr = &myblock[ind+sizeof(entry)*2 +size];
						currEntry->blockSize = size;
						currEntry->next = newEntry;
						currEntry->free = '0';
						return currEntry->dataPtr;
					} else {	//block is not big enough for splitting
						currEntry->free='0';
						currEntry->dataPtr = &myblock[ind+sizeof(entry)];
						return currEntry->dataPtr;
					}
				} else {
					if ( currEntry->next!=NULL ) {	//there are more nodes ahead
						ind = ind + currEntry->blockSize+sizeof(entry);
						currEntry = currEntry->next;	
					} else {
						int endOfList = (int)(ind+currEntry->blockSize+sizeof(entry));
						if ((int)(endOfList+size + sizeof(entry))>4096) {
							printf("%s: %d: Error: Not enough space left . Returning NULL\n", filename,lineNumber);
							return NULL;
						}
						anotherEntry = (entry*)&myblock[ind+sizeof(entry)+currEntry->blockSize];
						anotherEntry->next = currEntry->next;
						anotherEntry->free = '1'; 
						anotherEntry->next = NULL;
						k = (int)currEntry->blockSize;
						s = 4096-( ind + 2*(int)sizeof(entry)+k);
						anotherEntry->blockSize = s;
						if (anotherEntry->blockSize>=(int)size) {  //check if need to split
							anotherEntry->free='0';
							if (anotherEntry->blockSize>(int)(size+sizeof(entry) +1)) { 
								anotherEntry->blockSize = size; 
							}
							currEntry->next = anotherEntry;			//because of this linking, currEntry is now linked to anotherEntry
							int p = (int)(ind+currEntry->blockSize+2*sizeof(entry));
							anotherEntry->dataPtr = &(myblock[p]);
							// printf("%d\n",(int)(ind+currEntry->blockSize+2*sizeof(entry)));
							return anotherEntry->dataPtr;
						} else {
							printf("%s: %d: Error: Could not find enough memory. Returning NULL\n", filename,lineNumber);
							return NULL;
						}
					}
				} 
			}
		}		
	}
	return NULL;
}
	