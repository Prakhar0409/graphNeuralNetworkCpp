CC	= mpicc
CCFLAGS	= -O3 -std=gnu99 -g
#-Wall
LFLAGS	= -lm -lGNN #-lga

CCLIBFLAGS = -fpic

#LIBPATH = -L/usr/lib/
LIBPATH= -L$(CURDIR)/gnn
LIBPATHNEW= -L/scratch/work/agrawap1/ga1/ga/gnnTest/gnn
RPATH = -Wl,-rpath="$(CURDIR)/gnn"
RPATHNEW = -Wl,-rpath=/scratch/work/agrawap1/ga1/ga/gnnTest/gnn
INCPATH	= -I../




SRC	= ga.c element.c
INC	= $(SRC:.c=.h)
#OBJ = $(SRC:.c=.o)

default: makeGNN all 

makeGNN:
	cd gnn && make

all: static.exe 

#static compilation
static.exe: static.c 
	$(CC) $(CCFLAGS) $(INCPATH) -c static.c
	$(CC) $(CCFLAGS) $(LIBPATH) $(RPATH) -o static.exe static.o ../libga.a $(LFLAGS)

dynamic.exe: static.c 
	$(CC) $(CCFLAGS) $(INCPATH) -c static.c
	$(CC) $(CCFLAGS) -o dynamic.exe static.o $(LFLAGS) -L../ -lga

clean:
	rm -rf *.o *.exe
