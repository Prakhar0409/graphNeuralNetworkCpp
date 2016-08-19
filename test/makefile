CC	= mpicc
CCFLAGS	= -O3 -std=gnu99 -Wall
LFLAGS	= -lm

CCLIBFLAGS = -fpic

LIBPATH = -L/usr/lib/
INCPATH	= -I../



SRC	= ga.c element.c
INC	= $(SRC:.c=.h)
#OBJ = $(SRC:.c=.o)

default: all

all: static.exe dynamic.exe

#static compilation
static.exe: static.c 
	$(CC) $(CCFLAGS) $(INCPATH) -c static.c
	$(CC) $(CCFLAGS) -o static.exe static.o ../libga.a $(LFLAGS)

dynamic.exe: static.c 
	$(CC) $(CCFLAGS) $(INCPATH) -c static.c
	$(CC) $(CCFLAGS) -o dynamic.exe static.o $(LFLAGS) -L../ -lga

clean:
	rm -rf *.o *.exe
