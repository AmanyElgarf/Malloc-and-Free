# Malloc
In mymalloc.h :
	I don't know for sure if code in mymalloc.h is written correctly , as it uses macros. However, when I call malloc, the call is redirected to mymalloc, which is a good thing. In general, take a look at mymalloc.h and see if I use macros and '#define' as professor mentioned in lecture that I missed. I also know that we need to write something like #ifndef, to make sure we don't include same library more than once, but we can check that later.

:crescent_moon:
In mymalloc.c: 
	I defined struct , 'entry' , that will be our metadata. For each call to mymalloc, as long as there is enough free 
	memory in array to allocate, new entry will be created. Its fields are:
		-'blocksize'(how much memory it will hold , should be as much as user is asking)
		-'free' . Initially, as the new entry is created, 'free' is initialized to 0, which means memory is allocated
		in this block, and cannot be used. However it can become free, after the call to 'myfree()' In that case , 			that block can be used for future allocations again. 
		- 'next' - will be pointing to the next block . Initially 'next' is NULL


- When mymalloc is being called, we want to be able to see if it was called before or not (because if it wasn't , then
there are no entries in the array, and trying to check entry's fields will cause an error). That's why the code checks 
if myblock[0] is equal to some value (213 in the code). 
	- If it's not, than mymalloc is being called for the first time, so we initialize myblock[0] to 213 for future
	calls, and then allocate the memory, returning the pointer to it. 
	- If myblock[0] is 213, then we need to find block of space at least as large as what is asked for user,
	create metadata holding info about that block, and return pointer to it. Of course, we might still find that 
	there are no blocks of needed size , in which case we return NULL. 
	- We will probably need a function , something like "find_a_block()" that will go through the list of structs,
	finding a struct of at least needed size, which 'free' field is 1. Or , we might find, that at the end of the 
	list, there is still unused chunk of memory. I was thinking if we should have a value at the beginning of 
	'myblock' array, like myblock[0], that will indicate the size of the largest block available in the array . So
	each time when mymalloc is called, the function 'find_a_block()' will only be called if the size of the 
	largest available block is not smaller than how much memory user is asking for. That is not necessary feature,
	but probably can improve the performance, which I think is extra points, so we get there after we done with 
	everything else. 

Before allocating the memory, we should check ,if user is not asking for too much memory (larger than size of myblock-
metadata).If the user asks for too much , we should return NULL . 

free() function :
	- should check if the argument passed is actually a pointer, and exists in the 'myblock' array(Or some other 
	possible errors mentioned in description). If not, print some message . 
	- If argument is OK, then find the entry that holds the memory about it, set 'free' field to '1'. If there are
	any other free entries adjacent to it, we need to merge them together (like if there are 2 adjacent blocks 
	that were freed, of x and y bytes, we should merge them into single block of (x + y+sizeof(entry)), because we
	only need one metadata for one block )

Side notes: 

- Because myblock array is of type char, to have inside of it data of any other types(int , struct etc) we
need to cast it. 
- I used __attribute__((__packed__)) while defining struct, this allows us to save space ( the char value 
'free' will not be padded to 8 , so instead of 24, struct will only take 13 bytes.) :cat:

