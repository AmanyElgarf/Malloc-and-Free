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
	void* dataPtr;	
}entry;
void myfree(void *ptr,char* filename,int lineNumber){
    //freeing a pointer that wasn't allocated by malloc before
    int freedBytes = 0;
    if ((char*)ptr < (myblock+6) || (char*)ptr >= myblock+sizeof(myblock)){
        printf("%s: %d: Error: (%p) is either not a pointer or a pointer that  was not allocated by malloc before.\n", filename,lineNumber,ptr);
        return;
    }
    entry* head;
    head = (entry*)&myblock[6];
    entry* new = head;
    //checking if value passed is pointer to dataBlock
    while(new->dataPtr != ptr && new->next != NULL){ new = new->next; } 
    if(new->next == NULL && new->dataPtr != ptr){
        printf("%s: %d: Error: (%p) is either not a pointer or a pointer that  was not allocated by malloc before.\n", filename,lineNumber,ptr);
        return;
    }
    //freeing a pointer that was already freed
    if(new->free == '1'){
        printf("%s: %d: Error: pointer (%p) was freed before.\n",filename,lineNumber, ptr);
        return;
    }
    else{
        if(new->blockSize > 4069){
            printf("%s: %d: Error: You can't free a pointer of size more than 4090 because we are using the first six units to store inf\n",filename,lineNumber);
            return;
        }
        //add block space to total space
        freedBytes = freedBytes+new->blockSize;
        new->free = '1';
        //check for next block
        if(new->next != NULL){
            if(new->next->free == '1'){
                new->blockSize = new->blockSize + new->next->blockSize+sizeof(entry) ;
                freedBytes = new->blockSize;
                if(new->next->next != NULL){ new->next =new->next->next; }
                else{ new->next = NULL; }
         	}
        }
        entry* curr = head;
        if(curr != new){
            while(curr->next != new){ curr = curr->next; }
            if(curr->free =='1'){
                curr->blockSize = curr->blockSize + new->blockSize+sizeof(entry);
                freedBytes = curr->blockSize;
                if(curr->next->next != NULL){ curr->next =curr->next->next; }
                else{ curr->next = NULL;  }
            }
        }        
        printf("freeing %d bytes\n",freedBytes);
        printf("Success: Pointer (%p) was freed successfully.\n", ptr);
    }    
}
void *mymalloc(size_t size,char* filename,int lineNumber) {
	printf("mallocing %d bytes\n",(int)size);
	int k,s;
	if (!(myblock[0]==(char)35&& myblock[1]==(char)111&&myblock[2]==(char)52&&myblock[3]==(char)241)) {	
		myblock[0]=(char)35;
		myblock[1]=(char)111;
		myblock[2]=(char)52;
		myblock[3]=(char)241;	//mymalloc is called first time, so we need to assign value to mybloc[0]
		printf("The size of entry is %d\n",(int)sizeof(entry));
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
		first_entry.dataPtr = &myblock[6+21];
		*(entry*)(myblock+6) = first_entry;
		printf("Allocated %d bytes at %d position. Returning pointer %p\n",(int)size,(int)(6+sizeof(entry)), first_entry.dataPtr);
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
				if (currEntry->blockSize>=(size+sizeof(entry)) && currEntry->free=='1') {

					if (currEntry->blockSize>=(size+sizeof(entry)+1)) {	// if block is big enough to split into 2 blocks
						int a = currEntry->blockSize-(int)size-sizeof(entry);
						newEntry = (entry*)&myblock[ind+sizeof(entry)+size];
						newEntry->blockSize = a;
						newEntry->free='1';
						newEntry->next = currEntry->next;
						newEntry->dataPtr = &myblock[ind+sizeof(entry)*2 +size];
						currEntry->blockSize = size;
						currEntry->next = newEntry;
						currEntry->free = '0';
						printf("location of block after %d bytes block is %d.\n",currEntry->blockSize,(int)(ind+sizeof(entry)*2+size));
						printf("returning pointer %p for %d bytes\n",currEntry->dataPtr,currEntry->blockSize);
						return currEntry->dataPtr;
					} else {	//block is not big enough for splitting
						currEntry->free='0';
						currEntry->dataPtr = &myblock[ind+sizeof(entry)];
						printf("returning pointer %p for %d bytes\n",newEntry->dataPtr,newEntry->blockSize);
						printf("location of %d bytes block is %d.\n",newEntry->blockSize,(int)(ind+sizeof(entry)));
						return currEntry->dataPtr;
					}
				} else {
					if ( currEntry->next!=NULL ) {	//there are more nodes ahead
						ind = ind + currEntry->blockSize+sizeof(entry);
						currEntry = currEntry->next;	
					} else {
						printf("entry's size: %d,sizeof entry : %d\n",currEntry->blockSize,(int)sizeof(entry));
						int endOfList = (int)(ind+currEntry->blockSize+sizeof(entry));
						printf("at the end of the list. Ind is  %d, end of list is: %d\n", ind,(int)endOfList);	//reached the end of the list. Check if have enough memory at the end
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
						if (anotherEntry->blockSize>=size) {  //check if need to split
							anotherEntry->free='0';
							if (anotherEntry->blockSize>(size+sizeof(entry) +1)) { anotherEntry->blockSize = size; }
							currEntry->next = anotherEntry;			//because of this linking, currEntry is now linked to anotherEntry
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

int main(int argc, char* argv[]) {

	// ---------TESTCASE 1 :: freeing and allocating many times, checking if freed blocks are being stitched,
	//  if malloc reuses freed blocks etc:
	// int *a = malloc(1000);
	// *(a+1) = 5;
	// *(a+3)=765;
	// int *b = malloc(500);
	// *(b+100) = 200;
	// int *c = malloc(50);
	// *(c+20) = 10;
	// entry *e = (entry*)&myblock[6];
	// printf("---------------------------------------------------------------------------------------------\n");
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// free(a);
	// printf("\nafter a was freed: ------------------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// int *aa = malloc(150);
	// printf("\nafter aa was allocated:-------------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// free(c);
	// printf("\nafter c was freed: ----------------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// free(b);
	// printf("\nafter b was freed: ----------------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// int *d = malloc(1200);
	// printf("\nafter d was allocated: ------------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// free(d);
	// printf("\nafter d was freed: -----------------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// free(c);
	// printf("\nafter c were freed: ----------------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// a = malloc(888);
	// printf("\nafter a was reallocated: -----------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// b = malloc(700);
	// printf("\nafter b was reallocated: -----------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// free(b);
	// printf("\nafter b was freed: -----------------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");
	// b = malloc(200);
	// ////////
	// printf("\nafter b was reallocated: -----------------------------------------------------------------\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf(" %d/%c| D:%d bytes at %p ||",(int)sizeof(entry),e->free,e->blockSize,e->dataPtr);
	// 	e=e->next;
	// } while (e!=NULL);
	// printf("\n");




	// --------TESTCASE2 :: testing for not allocating when there is no more memory
	// int *b = malloc(1000);
	// int *c = malloc(2000);
	// int *d = malloc(1000);
	// int *e = malloc(5);
	// int *a = malloc(1);




	//-----------TESTCASE3 :: testing for allocating more bytes if there are less than 1+sizeof(entry) left at the end
	int *a = malloc(4040);
	int *b = malloc(1);


	return 0;
}


