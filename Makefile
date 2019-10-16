CC = gcc 
CFLAGS = -g 

all: memgrind

mymalloc: mymalloc.c
	$(CC) $(CFLAGS) -o mm mymalloc.c

memgrind: mymalloc.c memgrind.c 
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o memgrind memgrind.c mymalloc.o

clean: 
<<<<<<< HEAD
	rm -f *.o memgrind mymalloc.o
# =======
# CC = gcc
# CCFLAGS = -Wall

# all: memgrind

# memgrind: memgrind.c mymalloc.c mymalloc.h
# 	$(CC) $(CCFLAGS) -g -o memgrind memgrind.c

# clean:
# 	rm memgrind
# >>>>>>> a676ee8e6b5f24b01a348a7a9f9ae0cfc92811b0
=======
	rm -f *.o memgrind
>>>>>>> ce26c9a9e8b5d348bfc8b3315be2bab65a97fe02
