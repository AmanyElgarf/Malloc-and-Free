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
    if ((char*)ptr < (myblock+6) || (char*)ptr >= myblock+sizeof(myblock)){
        printf("%s: %d: Error: (%p) is either not a pointer or a pointer that  was not allocated by malloc before.\n", filename,lineNumber,ptr);
        return;
    }
     entry* head;
    head = (entry*)&myblock[6];
    entry* new = head;
    while(new->dataPtr != ptr && new->next != NULL){
        new = new->next;
    }
   
    if(new->next == NULL && new->dataPtr != ptr){
        printf("Error: (%p) is either not a pointer or a pointer that  was not allocated by malloc before.\n", ptr);
        return;
    }
    //freeing a pointer that was already freed
    if(new->free == '1'){
        printf("%s: %d: Error: pointer (%p) was freed before.\n",filename,lineNumber, ptr);
        return;
    }
    else{
    	// printf("line 36\n");
        if(new->blockSize > 4090){
            printf("%s: %d: Error: You can't free a pointer of size more than 4090 because we are using the first six units to store inf\n",filename,lineNumber);
            return;
        }
        //add block space to total space
        myblock[4] = (char)((int)myblock[4] + head->blockSize/100);
        myblock[5] = (char)((int)myblock[5] + head->blockSize%100);
        new->free = '1';
        //check for next block
        if(new->next != NULL){
            if(new->next->free == '1'){
                new->blockSize = new->blockSize + new->next->blockSize+sizeof(entry) ;
                if(new->next->next != NULL){ new->next =new->next->next; }
                else{ new->next = NULL; }
         	}
        }
        entry* curr = head;
        if(curr != new){
            while(curr->next != new){ curr = curr->next; }
            if(curr->free =='1'){
                curr->blockSize = curr->blockSize + new->blockSize+sizeof(entry);
                if(curr->next->next != NULL){ curr->next =curr->next->next; }
                else{ curr->next = NULL;  }
            }
        }        
        // entry* check = head;
        // int count = 0;
        // while(check!=NULL){
        //     if(check->free == '0'){
        //         count++;
        //     }
        //     check=check->next;
        // }
        // if(count == 0){
        //     myblock[0]=(char)345;
        //     myblock[1]=(char)1441;
        //     myblock[2]=(char)524;
        //     myblock[3]=(char)24;
        // }
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
			printf("%s: %d: Cannot allocate that amount of space, try asking for less.\n",filename,lineNumber);
			return NULL;
		} else if (size==0) {
			printf("%s: %d: error:\tCannot allocate for 0 bytes\n",filename,lineNumber);
			return NULL;
		}
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
			printf("%s: %d: Not enough memory. You have only %d bytes available\n",filename,lineNumber, (int)(memMinusMeta)); 
			return NULL;
		} else if (size==0) {	//in case if user asks 0 
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
						// newEntry->dataPtr = &myblock[ind+sizeof(entry)];	????????
						newEntry->dataPtr = &myblock[ind+sizeof(entry)*2 +size];
						currEntry->blockSize = size;
						currEntry->next = newEntry;
						currEntry->free = '0';
						num = myblock[4]*100+myblock[5];
						num =-(size+sizeof(entry));
						myblock[4] = (char)(num/100);
						myblock[5] = (char)(num%100);
						printf("returning pointer %p for %d bytes\n",currEntry->dataPtr,currEntry->blockSize);
						return currEntry->dataPtr;
					} else {	//block is not big enough for splitting
						currEntry->free='0';
						num = myblock[4]*100+myblock[5];
						num =-(currEntry->blockSize+sizeof(entry));
						myblock[4] = (char)(num/100);
						myblock[5] = (char)(num%100);
						currEntry->dataPtr = &myblock[ind+sizeof(entry)];
						printf("returning pointer %p for %d bytes\n",newEntry->dataPtr,newEntry->blockSize);
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
							if (anotherEntry->blockSize>(size+sizeof(entry) +1)) { anotherEntry->blockSize = size; }
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

int main(int argc, char* argv[]) {
	int *a = malloc(1000);
	int *b = malloc(500);
	int *c = malloc(50);
	entry *e = (entry*)&myblock[6];
	printf("\n");
	do {
		printf("size: %d, pointer: %p free: %c\n",e->blockSize, e->dataPtr,e->free);
		e=e->next;
	} while (e!=NULL);
	// free(a+6);
	free(a);
	int *aa = malloc(150);
	free(aa+5);
	// printf("\nafter a was freed: -----\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf("size: %d, pointer: %p free: %c\n",e->blockSize, e->dataPtr,e->free);
	// 	e=e->next;
	// } while (e!=NULL);
	// free(c);
	// printf("\nafter c was freed: -----\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf("size: %d, pointer: %p free: %c\n",e->blockSize, e->dataPtr,e->free);
	// 	e=e->next;
	// } while (e!=NULL);
	// free(b);
	// printf("\nafter b was freed: -----\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf("size: %d, pointer: %p free: %c\n",e->blockSize, e->dataPtr,e->free);
	// 	e=e->next;
	// } while (e!=NULL);
	// int *d = malloc(1200);
	// printf("\nafter d was allocated: -----\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf("size: %d, pointer: %p free: %c\n",e->blockSize, e->dataPtr,e->free);
	// 	e=e->next;
	// } while (e!=NULL);
	// free(d);
	// free(c);
	// printf("\nafter d and c were freed: -----\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf("size: %d, pointer: %p free: %c\n",e->blockSize, e->dataPtr,e->free);
	// 	e=e->next;
	// } while (e!=NULL);
	// a = malloc(888);
	// printf("\nafter a was reallocated: -----\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf("size: %d, pointer: %p free: %c\n",e->blockSize, e->dataPtr,e->free);
	// 	e=e->next;
	// } while (e!=NULL);
	// b = malloc(700);
	// printf("\nafter b was reallocated: -----\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf("size: %d, pointer: %p free: %c\n",e->blockSize, e->dataPtr,e->free);
	// 	e=e->next;
	// } while (e!=NULL);
	// free(b);
	// printf("\nafter b was freed: -----\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf("size: %d, pointer: %p free: %c\n",e->blockSize, e->dataPtr,e->free);
	// 	e=e->next;
	// } while (e!=NULL);
	// b = malloc(200);
	// printf("\nafter b was reallocated: -----\n");
	// e = (entry*)&myblock[6];
	// do {
	// 	printf("size: %d, pointer: %p free: %c\n",e->blockSize, e->dataPtr,e->free);
	// 	e=e->next;
	// } while (e!=NULL);
	return 0;
}


