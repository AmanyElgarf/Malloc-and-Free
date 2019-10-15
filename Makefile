<<<<<<< HEAD
# <<<<<<< HEAD
CC = gcc 
CFLAGS = -g -Wall 
=======
CC = g++
CCFLAGS = -Wall
>>>>>>> c109d14459dbc917c1d031e5a59a03e4a5751468

all: memgrind

mymalloc: mymalloc.c
	$(CC) $(CFLAGS) -o mm mymalloc.c

memgrind: mymalloc.c memgrind.c 
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o memgrind memgrind.c mymalloc.o

clean: 
	rm -f *.o memgrind
# =======
# CC = gcc
# CCFLAGS = -Wall

# all: memgrind

# memgrind: memgrind.c mymalloc.c mymalloc.h
# 	$(CC) $(CCFLAGS) -g -o memgrind memgrind.c

# clean:
# 	rm memgrind
# >>>>>>> a676ee8e6b5f24b01a348a7a9f9ae0cfc92811b0
