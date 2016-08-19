CC	= mpicc
CCFLAGS	= -O3 -std=gnu99 -Wall
LFLAGS	= -lm

CCLIBFLAGS = -fpic

LIBPATH = -L/usr/lib/
INCPATH	= 

LIB	= -lnetwork #-lvectors

SRC	= ga.c element.c
INC	= $(SRC:.c=.h)
#OBJ = $(SRC:.c=.o)
OBJ = $(addprefix static-lib/, $(patsubst %.c, %.o, $(wildcard *.c)))
OBJD= $(addprefix shared-lib/, $(patsubst %.c, %.o, $(wildcard *.c)))

#vpath %.h ./vectors/
#vpath %.h ./networks/

default: all

all: static-lib libga.a shared-lib libga.so

#static compilation
static-lib:
	mkdir static-lib

static-lib/%.o: %.c $(INC)
	$(CC) $(CCFLAGS) $(INCPATH) -c $< -o $@
	@echo $(OBJ)

libga.a: $(OBJ)
	ar rcs libga.a $(OBJ)
	cp libga.a ../.
	cp libga.a ~/lib/.

shared-lib:
	mkdir shared-lib

shared-lib/%.o: %.c $(INC)
	$(CC) $(CCFLAGS) $(CCLIBFLAGS) $(INCPATH) -c $< -o $@
	@echo $(OBJ)

libga.so: $(OBJD)
	$(CC) $(CCFLAGS) -shared $(INCPATH) -o libga.so $(OBJD) $(LFLAGS) -lpthread
	@cp libga.so ../.
	@cp libga.so ~/lib/.


clean:
	rm -rf static-lib
	rm -rf shared-lib
	rm *.a *.so
