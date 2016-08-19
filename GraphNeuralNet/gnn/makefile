CC = g++
CCLIBFLAGS = -fPIC

LIBPATH = -L/u/20/agrawap1/unix/Desktop/ga/ga/gnnTest

all: static dynamic

static: molecule neuralNet readData learn
	ar rcs libGNN.a molecule.o learn.o readData.o neural.o

dynamic: molecule neuralNet readData learn
	$(CC) -shared -o libGNN.so molecule.o learn.o readData.o neural.o

molecule: molecule.cpp
	$(CC) $(CCLIBFLAGS) -c molecule.cpp

neuralNet: neural.cpp
	$(CC) $(CCLIBFLAGS) -c neural.cpp
	
learn: learn.cpp
	$(CC) $(CCLIBFLAGS) -c learn.cpp

readData: readData.cpp
	$(CC) $(CCLIBFLAGS) -c readData.cpp

clean:
	rm *.o
